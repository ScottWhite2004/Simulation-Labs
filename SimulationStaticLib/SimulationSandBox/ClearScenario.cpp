#include "ClearScenario.h"


ClearScenario::~ClearScenario()
{
}

void ClearScenario::OnLoad()
{
}

void ClearScenario::OnUpdate(float seconds)
{
}

void ClearScenario::OnUnload()
{
}

void ClearScenario::ImGuiMain()
{
	if (ImGui::BeginMenu("Colour"))
	{
		ImGui::ColorPicker4("Clear Colour", (float*)&_colour);
		ImGui::EndMenu();
	}
}
