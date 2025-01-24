#include "framework.h"

void Initialize()
{
    AllocConsole();
    FILE* File;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    
    UGameEngine* Engine = UFortEngine::GetEngine();
    Logging::Log(ELogEvent::Info, ELogType::Game, "Engine: %s \n", Engine->GetFullName().c_str());

    FString Map = L"open Athena_Terrain";
    UKismetSystemLibrary::ExecuteConsoleCommand(Engine->GameViewport->World, Map, nullptr);
    Logging::Log(ELogEvent::Info, ELogType::Kismet, "Executing Command: %s \n", Map.ToString().c_str());

    Engine->GameInstance->LocalPlayers.Remove(0);

    Logging::Log(ELogEvent::Info, ELogType::Hook, "Starting Hooks... \n");
    MH_Initialize();
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

