// MusicZoneComponent.cpp
// Level-based music zones driving adaptive music states.

#include "MusicZoneComponent.h"
#include "MusicSubsystem/MusicSystem.h"
#include "MusicSubsystem/MusicAsset.h"

void UMusicZoneComponent::HandleZoneEntered()
{
    if (!AreMusicAssetsValid() || !MusicSystem.IsValid())
        return;

    // If previous music is already stopping, switch immediately
    if (MusicSystem->IsStoppingMusicEquals(MusicAssetOnEnter))
    {
        if (!bIsActive)
        {
            MusicSystem->PlayMusic(MusicAreaOnEnter);
            bIsActive = true;
        }
    }
    else
    {
        // Gate-based delayed transition
        ZoneTimer->StartZoneInTimer(MusicAssetOnEnter->GateTimeIn);
    }
}

void UMusicZoneComponent::HandleZoneExited()
{
    if (!AreMusicAssetsValid())
        return;

    // Controlled fade-out using gate timings
    ZoneTimer->StartZoneOutTimer(
        MusicAssetOnEnter->GateTimeIn,
        MusicAssetOnEnter->GateTimeOut
    );
}

void UMusicZoneComponent::HandleZoneEnterConfirmed()
{
    if (!bIsActive && MusicSystem.IsValid())
    {
        MusicSystem->PlayMusic(MusicAreaOnEnter);
        bIsActive = true;
    }
}

void UMusicZoneComponent::HandleZoneExitConfirmed()
{
    if (bIsActive && MusicSystem.IsValid())
    {
        MusicSystem->StopMusic(MusicAreaOnEnter);
        bIsActive = false;
    }
}