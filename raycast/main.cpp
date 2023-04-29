#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "map.h"
#include "constants.h"

void DrawDebugUI(Map& world, Camera& camera, DrawMode& mode)
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::BulletText("First of all, you need to draw the walls on the map using the checkbox \"Enable drawing\"");
		ImGui::BulletText("WASD to move. <- and -> to rotate.");
	}
	ImGui::SeparatorText("Drawing");

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
	ImGui::RadioButton("None", reinterpret_cast<int*>(&mode), 0);
	ImGui::SameLine();
	ImGui::RadioButton("Rectangle", reinterpret_cast<int*>(&mode), 1);
	ImGui::SameLine();
	ImGui::RadioButton("Line", reinterpret_cast<int*>(&mode), 2);
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

	ImGui::End();
}


int main()
{
	sf::RenderWindow window_2d(sf::VideoMode{WINDOW_WIDTH_2d, WINDOW_HEIGHT_2d}, "2d");
	sf::RenderWindow window_3d(sf::VideoMode{WINDOW_WIDTH, WINDOW_WIDTH}, "3d");

	window_2d.setPosition({window_2d.getPosition().x + static_cast<int>(WINDOW_WIDTH) / 2, window_2d.getPosition().y});
	window_3d.setPosition({window_3d.getPosition().x - static_cast<int>(WINDOW_WIDTH) / 2, window_3d.getPosition().y});

	ImGui::SFML::Init(window_2d);
	sf::Clock clock;

	Map world;
	Camera camera(50, 600, world);
	float deltaTime = 0.f;
	DrawMode mode{0};

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

		DrawDebugUI(world, camera, mode);

		camera.Update(deltaTime);

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			world.Update(event, window_2d, mode);
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
