#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "map.h"
#include "constants.h"

int main()
{
	sf::RenderWindow window_2d(sf::VideoMode(static_cast<int32_t>(WINDOW_WIDTH_2d), static_cast<int32_t>(WINDOW_HEIGHT_2d)),
	                           "2d", sf::Style::None);
	sf::RenderWindow window_3d(sf::VideoMode(static_cast<int32_t>(WINDOW_WIDTH), static_cast<int32_t>(WINDOW_HEIGHT)), "3d");

	window_2d.setPosition({window_2d.getPosition().x + static_cast<int>(WINDOW_WIDTH) / 2, window_2d.getPosition().y});
	window_3d.setPosition({window_3d.getPosition().x - static_cast<int>(WINDOW_WIDTH) / 2, window_3d.getPosition().y});

	ImGui::SFML::Init(window_2d);
	sf::Clock clock;

	Map world;
	Camera camera(50, 600, world);
	float deltaTime = 0.f;

	bool map_is_drawing = false;

	while (window_2d.isOpen() && window_3d.isOpen())
	{
		sf::Event event;
		while (window_2d.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window_2d.close();
		}
		while (window_3d.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window_3d.close();
		}

		deltaTime = clock.getElapsedTime().asSeconds();
		ImGui::SFML::Update(window_2d, clock.restart());
		ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Once);
		ImGui::Begin("Settings", nullptr,
		             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);

		if (ImGui::CollapsingHeader("Help"))
		{
			ImGui::BulletText("First of all, you need to draw the walls on the map using the checkbox \"Enable drawing\"");
			ImGui::BulletText("WASD to move. <- and -> to rotate.");
		}
		ImGui::SeparatorText("Drawing");
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Checkbox("Enable drawing", &map_is_drawing);
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Hold down the left mouse button on 2d view to draw");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		if (ImGui::Button("Delete all"))
		{
			world.DeleteMap();
		}
		if (!map_is_drawing)
		{
			ImGui::SeparatorText("Variables");
			{
				int nor = camera.GetNumberOfRays();
				ImGui::SliderInt("Number of rays", &nor, 10, MAX_NUMBER_OF_RAYS);
				camera.SetNumberOfRays(nor);
			}
			{
				float horizontal_fov = camera.GetHorizotalFOV();
				ImGui::SliderFloat("Horizontal FOV", &horizontal_fov, 1, 179);
				camera.SetHorizotalFOV(horizontal_fov);
			}
			{
				float vertical_fov = camera.GetVerticalFOV();
				ImGui::SliderFloat("Vertical FOV", &vertical_fov, 1, 179);
				camera.SetVerticalFOV(vertical_fov);
			}
			{
				float vision_range = camera.GetVisionRange();
				ImGui::SliderFloat("Vision range", &vision_range, 1, 2000);
				camera.SetVisionRange(vision_range);
			}
			ImGui::SeparatorText("Colors");
			{
				auto color = camera.GetSkyColor();
				float imgui_color[3] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f};
				ImGui::ColorEdit3("Sky", imgui_color, ImGuiColorEditFlags_NoInputs);
				camera.SetSkyColor((uint8_t)(imgui_color[0] * 255), (uint8_t)(imgui_color[1] * 255),
				                   (uint8_t)(imgui_color[2] * 255));
			}
			{
				auto color = camera.GetGroundColor();
				float imgui_color[3] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f};
				ImGui::ColorEdit3("Ground", imgui_color, ImGuiColorEditFlags_NoInputs);
				camera.SetGroundColor((uint8_t)(imgui_color[0] * 255), (uint8_t)(imgui_color[1] * 255),
				                      (uint8_t)(imgui_color[2] * 255));
			}
		}

		ImGui::End();


		camera.Update(deltaTime);

		if (map_is_drawing && !io.WantCaptureMouse)
		{
			world.Update(event, window_2d);
		}

		window_2d.clear();
		window_3d.clear();


		camera.Draw(window_2d, window_3d);
		world.Draw2d(window_2d);
		ImGui::SFML::Render(window_2d);

		window_2d.display();
		window_3d.display();
	}
	ImGui::SFML::Shutdown();
}
