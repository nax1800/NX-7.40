#pragma once
#include "framework.h"

namespace GameMode
{
	bool bIsPlaylistSetup = false;

	bool (*oReadyToStartMatch)(AGameMode*);
	bool hkReadyToStartMatch(AFortGameModeAthena* GameMode)
	{
		auto GameState = static_cast<AFortGameStateAthena*>(GameMode->GameState);

		if (!bIsPlaylistSetup)
		{
			bIsPlaylistSetup = true;
			auto Playlist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
			if (Playlist)
			{
				Logging::Log(ELogEvent::Info, ELogType::Game, "Playlist is setting up...");
				GameState->CurrentPlaylistId = Playlist->PlaylistId;
				FPlaylistPropertyArray& PlaylistInfo = GameState->CurrentPlaylistInfo;
				PlaylistInfo.BasePlaylist = Playlist;
				PlaylistInfo.OverridePlaylist = Playlist;
				PlaylistInfo.PlaylistReplicationKey++;
				PlaylistInfo.MarkArrayDirty();

				GameMode->CurrentPlaylistName = Playlist->PlaylistName;
				GameMode->CurrentPlaylistId = Playlist->PlaylistId;
				GameState->OnRep_CurrentPlaylistId();
				GameState->OnRep_CurrentPlaylistInfo();

				GameState->FriendlyFireType = Playlist->FriendlyFireType;
				GameState->AirCraftBehavior = Playlist->AirCraftBehavior;
				GameMode->GameSession->MaxPlayers = Playlist->MaxPlayers;

				GameState->OnRep_AdditionalPlaylistLevelsStreamed();
			}
		}

		if (!GameState->MapInfo)
			return false;

		if (false) // is listening
		{
			GameMode->WarmupRequiredPlayerCount = 1;
			GameState->OnRep_CurrentPlaylistInfo();
		}

		GameMode->bWorldIsReady = true;

		return oReadyToStartMatch(GameMode);
	}

	APawn* hkSpawnDefaultPawnFor(AGameMode* GameMode, AController* NewPlayer, AActor* StartSpot)
	{
		if (!NewPlayer || !StartSpot)
			return nullptr;

		FTransform SpawnTransform = StartSpot->GetTransform();
		return GameMode->SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
	}

	void Initialize()
	{
		MH_STATUS RTSMStatus = Memory::CreateHook(Memory::GetAddress(0x2d44e00), hkReadyToStartMatch, (void**)&oReadyToStartMatch);
		MH_STATUS SDPFStatus = Memory::CreateHook(Memory::GetAddress(0xd58fc0), hkSpawnDefaultPawnFor);

		Logging::Log(ELogEvent::Info, ELogType::Hook, "hkReadyToStartMatch status: %s", MH_StatusToString(RTSMStatus));
		Logging::Log(ELogEvent::Info, ELogType::Hook, "hkSpawnDefaultPawnFor status: %s", MH_StatusToString(SDPFStatus));
	}
}