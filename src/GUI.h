#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "UUID.h"

namespace ImGui
{
	static void Spacing(float f)
	{
		ImGui::Dummy({ 0, f });
		ImGui::Separator();
		ImGui::Dummy({ 0, f });
	}

	static std::string GetUniqueName(std::string name, std::string id)
	{
		return name + "##" + id;
	}
}