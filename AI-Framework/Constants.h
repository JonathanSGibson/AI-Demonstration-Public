#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

//Screen dimensions
constexpr int ScreenWidth = 32*16*3;
constexpr int ScreenHeight = 32*9*3;

//BasEntity stuff
constexpr float MaxBoidSpeed = 4.0f;

constexpr float ForceLineMultiplier = 50.0f;
constexpr int FleeDetectionRadius = 200;

//Functions for translating between degrees and radians
constexpr double RadToDeg = 180 / M_PI;
constexpr double DegToRad = M_PI / 180;

//Grid cell size
constexpr int MaxCellWidth = 32;
constexpr int MaxCellHeight = 32;

//Wandering circle
constexpr float CircleRadius = 50.0f;
constexpr float AgentToCentre = CircleRadius * 1.5f;

//Rat stuff
constexpr int RatFleeMultiplier = 1;

//Object stuff
constexpr int HauntedObjectTotal = 3;

//Miner stuff
constexpr float PathfindingTolerance = 2.0f;
constexpr float ObjectSearchRadius = 300.0f;

//Ghost
const float WanderWeighting = 1.0f;
const float FleeWeighting = 1.0f;
const float PosessionRange = 5.0f;
