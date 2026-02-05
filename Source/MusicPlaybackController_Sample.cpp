// MusicPlaybackController.cpp
// Central controller responsible for buffered music playback
// and FMOD marker-driven transitions.

#include "MusicPlaybackController.h"
#include "FMODStudio/Classes/FMODAudioComponent.h"
#include "MusicSubsystem/MusicAsset.h"

void AMusicPlaybackController::PlayBufferedMusic()
{
    if (!NextMusicBuffer.IsValid())
        return;

    // Move buffered music into the active slot
    StoppingMusic.Empty();
    CurrentMusic = NextMusicBuffer;
    NextMusicBuffer.Empty();

    BindMusicPlayerDelegates(CurrentMusicPlayer);

    CurrentMusicPlayer->SetEvent(CurrentMusic.MusicAsset->GetMusicEvent());
    CurrentMusicPlayer->Play();

    // Optional re-queue based on FMOD user property
    if (ShouldReplayEvent(CurrentMusic.MusicAsset->GetMusicEvent()))
        NextMusicBuffer = CurrentMusic;
}

void AMusicPlaybackController::OnTimelineMarker(const FString& MarkerName, int32 Position)
{
    // Marker-based control over musical form
    if (MarkerName.Equals(TEXT("MusicNext")))
    {
        SwapMusicPlayers();
        UnbindMusicPlayerDelegates(StoppingMusicPlayer);

        if (NextMusicBuffer.IsValid())
            PlayBufferedMusic();
    }
}

bool AMusicPlaybackController::ShouldReplayEvent(UFMODEvent* Event) const
{
    // Controlled via FMOD user properties (implementation omitted)
    return false;
}