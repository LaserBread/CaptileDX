/* AUDIOPLAYER (Source) - Ethan Jones - audioplayer
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "audioengine.hpp"
#include "miniaudio.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include "sound.hpp"
/*===========================================================================*/

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

/*======== AUDIOENGINE: CONSTRUCTOR =========================================*/
AudioEngine::AudioEngine() {
    ma_result r = ma_engine_init(nullptr, &_engine);
    std::cout << "AudioEngine: " << ma_result_description(r) << std::endl;
    if ( r == MA_SUCCESS) {
        _initialized = true;
    }
}
/*===========================================================================*/

/*======== AUDIOENGINE: DESTRUCTOR ==========================================*/
//
AudioEngine::~AudioEngine() {
    // Destroy all sounds
    _sounds.clear();

    ma_engine_uninit(&_engine);
}
/*===========================================================================*/

/*======== AUDIOENGINE: REGISTER SOUND EFFECT ===============================*/
// Register a new `SoundEffect` to be played.
// @returns `true` on success, `false` on failure.
// @param name The name of this SoundEffect. Used to reference when playing
// sounds. This name must not already exist or registration will fail.
// @param path The path to the audio file. Supports WAV, FLAC, and MP3.
// @param poolSize When an effect can be played multiple times and overlap is
// desired, a copy of the sound may be played. Specifies how many copies exist
bool AudioEngine::registerEffect(std::string name, const std::string& path, std::size_t poolSize) {
    if (!_initialized) return false;
    auto it = std::lower_bound(
        _sounds.begin(), _sounds.end(), name,
        [](const SoundEffect& effect, const std::string& targetName) {
            return effect.name() < targetName;
        });

    // This name already exists. Fail.
    if (it != _sounds.end() && it->name() == name) {
        return false;
    }

    _sounds.emplace_back(name, &_engine, path, poolSize);
    std::sort(_sounds.begin(), _sounds.end());

    return true;
}
/*===========================================================================*/

/*======== AUDIOENGINE: UNREGISTER SOUND EFFECT =============================*/
// Unregister a `SoundEffect` matching the name and deallocate its resources.
// Currently-playing instances of the `SoundEffect` are stopped.
// @returns `true` if the `SoundEffect` was deleted. `false` if no matching
// `SoundEffect` could be found.
// @param name Name of the `SoundEffect` to be unregistered.
bool AudioEngine::unregisterEffect(const std::string& name){
    if (!_initialized) return false;
    auto it = std::lower_bound(
        _sounds.begin(), _sounds.end(), name,
        [](const SoundEffect& effect, const std::string& targetName) {
            return effect.name() < targetName;
        });

    // We found a sound. Delete it.
    if (it != _sounds.end() && it->name() == name) {
        _sounds.erase(it);
        return true;
    }

    return false;
}
/*===========================================================================*/

/*======== AUDIOENGINE: PLAY EFFECT =========================================*/
// Plays a linked `SoundEffect`
// @returns `true` if the `SoundEffect` was played. `false` if no
// `SoundResource`s were available to play or no matching `SoundEffect` could
// be found.
// @param name Name of the `SoundEffect` to be played.
bool AudioEngine::playEffect(const std::string& name){
    if (!_initialized) return false;
    auto it = std::lower_bound(
        _sounds.begin(), _sounds.end(), name,
        [](const SoundEffect& effect, const std::string& targetName) {
            return effect.name() < targetName;
        });

    // We found a sound. Play it.
    if (it != _sounds.end() && it->name() == name) {
        return it->playFromStart(&_engine);
    }

    return false;
}
/*===========================================================================*/

/*======== AUDIOENGINE: STOP EFFECT =========================================*/
// Stops a linked `SoundEffect`
// @returns `true` if the `SoundEffect` was stopped. `false` if no
//  matching `SoundEffect` could be found.
// @param name Name of the `SoundEffect` to be stopped.
bool AudioEngine::stopEffect(const std::string& name){
    if (!_initialized) return false;
    auto it = std::lower_bound(
        _sounds.begin(), _sounds.end(), name,
        [](const SoundEffect& effect, const std::string& targetName) {
            return effect.name() < targetName;
        });

    // We found a sound. Stop it.
    if (it != _sounds.end() && it->name() == name) {
        it->stopAll();
        return true;
    }

    return false;
}
/*===========================================================================*/
