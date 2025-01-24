#include "framework.h"

void Initialize()
{
    AllocConsole();
    FILE* File;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    
    Logging::Log(ELogEvent::Info, ELogType::NX, "Globals::bUseBeacons: %s", Globals::bUseBeacons ? "true" : "false");
    Logging::Log(ELogEvent::Info, ELogType::NX, "Globals::Port: %i", Globals::Port);

    UGameEngine* Engine = UFortEngine::GetEngine();
    Logging::Log(ELogEvent::Info, ELogType::Game, "Engine: %s", Engine->GetFullName().c_str());

    FString Map = L"open Athena_Terrain";
    UKismetSystemLibrary::ExecuteConsoleCommand(Engine->GameViewport->World, Map, nullptr);
    Logging::Log(ELogEvent::Info, ELogType::Kismet, "Executing Command: %s", Map.ToString().c_str());

    Engine->GameInstance->LocalPlayers.Remove(0);

    MH_STATUS InitializeStatus =  MH_Initialize();
    Logging::Log(ELogEvent::Info, ELogType::Hook, "Initialized MinHook, Status: %s", MH_StatusToString(InitializeStatus));

    GameMode::Initialize();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, LPVOID lpReserved)
{
    switch (ulReason)
    {
    case DLL_PROCESS_ATTACH:
        Initialize();
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

