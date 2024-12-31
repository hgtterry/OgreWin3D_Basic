#pragma once

#include "resource.h"
#include "CL64_App.h"

void Brush_SetSubtract(Brush* b, const signed int bState);
void Brush_SetHollow(Brush* b, const signed int bState);
void Brush_SetHullSize(Brush* b, const float hsize);
void Brush_SetGroupId(Brush* b, const int gid);