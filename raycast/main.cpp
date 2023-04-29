#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "map.h"
#include "constants.h"


void DrawDebugUI(Map& world, Camera& camera)
{
	using namespace ImGui;

	// SetNextWindowPos({0, 0}, ImGuiCond_Once);

	Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

	if (CollapsingHeader("Help"))
	{
		BulletText("First of all, you need to draw the walls on the map using the checkbox \"Enable drawing\"");
		BulletText("WASD to move. <- and -> to rotate.");
	}
	SeparatorText("Drawing");

	// Checkbox("Enable drawing", &map_is_drawing);
	SameLine();
	TextDisabled("(?)");

	if (IsItemHovered(ImGuiHoveredFlags_DelayShort) && BeginTooltip())
	{
		PushTextWrapPos(GetFontSize() * 35.0f);
		TextUnformatted("Hold down the left mouse button on 2d view to draw");
		PopTextWrapPos();
		EndTooltip();
	}

	if (Button("Delete all"))
	{
		world.DeleteMap();
	}

	// if (!map_is_drawing)
	{
		SeparatorText("Variables");
		{
			int nor = camera.GetNumberOfRays();
			SliderInt("Number of rays", &nor, 10, MAX_NUMBER_OF_RAYS);
			camera.SetNumberOfRays(nor);
		}
		{
			float horizontal_fov = camera.GetHorizotalFOV();
			SliderFloat("Horizontal FOV", &horizontal_fov, 1, 179);
			camera.SetHorizotalFOV(horizontal_fov);
		}
		{
			float vertical_fov = camera.GetVerticalFOV();
			SliderFloat("Vertical FOV", &vertical_fov, 1, 179);
			camera.SetVerticalFOV(vertical_fov);
		}
		{
			float vision_range = camera.GetVisionRange();
			SliderFloat("Vision range", &vision_range, 1, 2000);
			camera.SetVisionRange(vision_range);
		}

		SeparatorText("Colors");
		{
			auto color = camera.GetSkyColor();
			float imgui_color[3] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f};
			ColorEdit3("Sky", imgui_color, ImGuiColorEditFlags_NoInputs);
			camera.SetSkyColor((uint8_t)(imgui_color[0] * 255), (uint8_t)(imgui_color[1] * 255),
			                   (uint8_t)(imgui_color[2] * 255));
		}
		{
			auto color = camera.GetGroundColor();
			float imgui_color[3] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f};
			ColorEdit3("Ground", imgui_color, ImGuiColorEditFlags_NoInputs);
			camera.SetGroundColor((uint8_t)(imgui_color[0] * 255), (uint8_t)(imgui_color[1] * 255),
			                      (uint8_t)(imgui_color[2] * 255));
		}
	}

	ImGui::End();
}


int main()
{
	sf::Vector2i initial_window_resolution = {static_cast<int32_t>(WINDOW_WIDTH_2d), static_cast<int32_t>(WINDOW_HEIGHT_2d)};

	sf::RenderWindow editor_window(sf::VideoMode{WINDOW_WIDTH_2d, WINDOW_HEIGHT_2d}, "editor");
	sf::RenderWindow game_window(sf::VideoMode{WINDOW_WIDTH, WINDOW_WIDTH}, "game");

	editor_window.setPosition(
	    {editor_window.getPosition().x + static_cast<int>(WINDOW_WIDTH) / 2, editor_window.getPosition().y});
	game_window.setPosition({game_window.getPosition().x - static_cast<int>(WINDOW_WIDTH) / 2, game_window.getPosition().y});

	ImGui::SFML::Init(editor_window);
	sf::Clock clock;

	Map world;
	Camera camera(50, 600, world);
	float delta_time_s = 0.f;

	while (editor_window.isOpen() && game_window.isOpen())
	{
		const auto& deltaClock = clock.restart();
		delta_time_s = deltaClock.asSeconds();

		sf::Event event;
		while (editor_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(editor_window, event);
			if (event.type == sf::Event::Closed)
				editor_window.close();
		}
		while (game_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				game_window.close();
		}

		ImGui::SFML::Update(editor_window, deltaClock);

		DrawDebugUI(world, camera);

		camera.Update(delta_time_s);

		if (!ImGui::GetIO().WantCaptureMouse)
			world.Update(event, editor_window);

		editor_window.clear();
		game_window.clear();

		camera.Draw(editor_window, game_window);
		world.Draw2d(editor_window);
		ImGui::SFML::Render(editor_window);

		editor_window.display();
		game_window.display();
	}
	ImGui::SFML::Shutdown();
}
