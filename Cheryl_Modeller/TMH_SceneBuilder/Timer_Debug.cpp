#include "pch.h"
#include "CL64_App.h"
#include "Timer_Debug.h"

Timer_Debug::Timer_Debug(void)
{
	start = std::chrono::high_resolution_clock::now();
}

Timer_Debug::~Timer_Debug(void)
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	float ms = duration.count() * 1000.0f;

	App->CL_ImGui_Dialogs->Debug_Float = ms;
	App->CL_ImGui_Dialogs->flag_Show_Debug_Timer = true;
}
