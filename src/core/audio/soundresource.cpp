/* AUDIOPLAYER (Source) - Ethan Jones - audioplayer
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "sound.hpp"
#include <iostream>
#include <memory>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
/*===========================================================================*/

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

/*======== SOUNDRESOURCE: CONSTRUCTOR =======================================*/
// @param engine The `AudioEngine` to bind to
// @param filePath The file to load. Supports WAV, FLAC, and MP3.
// @param flags What flags to use.
// @param group What sound group to assign to.
SoundResource::SoundResource(
    ma_engine* engine,
    const char* filePath,
    ma_uint32 flags,
    ma_sound_group* group) {
    if (!engine || !filePath) {
        return;
    }
    auto sound = std::make_unique<ma_sound>();
    ma_result r = ma_sound_init_from_file(engine, filePath, flags, group, nullptr, sound.get());
    std::cout << "SoundResource constructor: " << ma_result_description(r) << std::endl;

    /*-------- STORE CONSTRUCTED SOUND --------------------------------------*/
    if (r == MA_SUCCESS) {
        _sound = std::move(sound);
        _initialized = true;
        _finished.store(true, std::memory_order_release);
        ma_sound_set_end_callback(_sound.get(), &SoundResource::onSoundEnd, this);
    }
    /*-----------------------------------------------------------------------*/
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: CLONE FROM ========================================*/
// Shared-resource copy from an existing template SoundResource.
// @param engine The `AudioEngine` to bind to
// @param templateSound The `SoundResource` to clone from
// @param flags What flags to use.
// @param group What sound group to assign to.
SoundResource SoundResource::cloneFrom(ma_engine* engine, SoundResource& templateSound,
                                       ma_uint32 flags, ma_sound_group* group) {
    SoundResource copy;
    if (!engine || !templateSound._initialized || !templateSound._sound) {
        return copy;
    }
    auto sound = std::make_unique<ma_sound>();
    ma_result r = ma_sound_init_copy(engine, templateSound._sound.get(), flags, group, sound.get());
    std::cout << "SoundResource clone: " << ma_result_description(r) << std::endl;

    /*-------- STORE CONSTRUCTED SOUND --------------------------------------*/
    if (r == MA_SUCCESS) {
        copy._sound = std::move(sound);
        copy._initialized = true;
        copy._finished.store(true, std::memory_order_release);
        ma_sound_set_end_callback(copy._sound.get(), &SoundResource::onSoundEnd, &copy);
    }
    /*-----------------------------------------------------------------------*/

    return copy;
}

/*======== SOUNDRESOURCE: MOVE CONSTRUCTOR ==================================*/
//
SoundResource::SoundResource(SoundResource&& other) noexcept {
    moveFrom(other);
}
/*%%%%%*/

/*======== SOUNDRESOURCE: ASSIGNMENT OPERATOR OVERLOAD ======================*/
//
SoundResource& SoundResource::operator=(SoundResource&& other) noexcept {
    if (this != &other) {
        reset();
        moveFrom(other);
    }
    return *this;
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: DESTRUCTOR ========================================*/
//
SoundResource::~SoundResource() {
    reset();
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: STOP SOUND ========================================*/
// Stops the currently playing sound and sets the finished flag.
void SoundResource::stop() {
    if (_initialized && _sound) {
        ma_sound_stop(_sound.get());
    }
    _finished.store(true, std::memory_order_release);
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: RESET =============================================*/
// Destroys all data associated with this `SoundResource` and reset it.
void SoundResource::reset() {
    if (!_initialized || !_sound) {
        _sound.reset();
        _initialized = false;
        _finished.store(true, std::memory_order_release);
        return;
    }
    // Stop playback and clear callback before uninitializing.
    stop();
    ma_sound_set_end_callback(_sound.get(), nullptr, nullptr);
    ma_sound_uninit(_sound.get());

    _sound.reset();
    _initialized = false;
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: IS FINISHED =======================================*/
// Check if this `SoundResource` has finished playing and ready to be recycled.
// @returns `true` if finished, `false` if not
bool SoundResource::isFinished() const {
    return _finished.load(std::memory_order_acquire);
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: UPDATE FINISHED STATE =============================*/
// Updates the finished state per frame
void SoundResource::updateFinishedState() {
    if (_initialized && _sound && !_finished.load(std::memory_order_acquire)) {
        if (ma_sound_at_end(_sound.get()) || !ma_sound_is_playing(_sound.get())) {
            _finished.store(true, std::memory_order_release);
        }
    }
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: PLAY FROM START ===================================*/
// Play a sound effect from the beginning.
// @returns `true` if the sound effect was able to start, `false` if it wasn't.
bool SoundResource::playFromStart() {
    if (!_initialized || !_sound) {
        return false;
    }
    ma_sound_seek_to_pcm_frame(_sound.get(), 0);
    _finished.store(false, std::memory_order_release);

    if (ma_sound_start(_sound.get()) != MA_SUCCESS) {
        _finished.store(true, std::memory_order_release);
        return false;
    }
    return true;
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: IS VALID? =========================================*/
// Verifies if a sound is initialized.
// @returns `true` if so, `false` if not.
bool SoundResource::isValid() const {
    return _initialized && (_sound != nullptr);
}
/*===========================================================================*/

/*======== SOUNDRESOURCE: ON SOUND END ====================== STATIC ========*/
// Callback for miniaudio to come set the finished flag when a sound is done
void SoundResource::onSoundEnd(void* pUserData, ma_sound* /*pSound*/) {
    auto* self = static_cast<SoundResource*>(pUserData);
    if (self) {
        // Keep callback lock-free and minimal.
        self->_finished.store(true, std::memory_order_release);
    }
}
/*===========================================================================*/

/*%%%SOUNDRESOURCE: MOVE FROM%%*/
// Move data from one SoundResource to another.
void SoundResource::moveFrom(SoundResource& other) noexcept {
    _sound = std::move(other._sound);
    _initialized = other._initialized;
    _finished.store(other._finished.load(std::memory_order_acquire), std::memory_order_release);

    other._initialized = false;
    other._finished.store(true, std::memory_order_release);

    if (_initialized && _sound) {
        // Re-point user data to the new wrapper object address (the ma_sound heap address is unchanged).
        ma_sound_set_end_callback(_sound.get(), &SoundResource::onSoundEnd, this);
    }
}
/*===========================================================================*/
