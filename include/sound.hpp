/* AUDIOPLAYER (Header) - Ethan Jones - audioplayer
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef SOUND_H
#define SOUND_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <atomic>
#include <string>
#include <memory>
#include <vector>

#include "miniaudio.h"
/*===========================================================================*/

/*======== SOUND RESOURCE ===================================================*/
// A container to abstract the playing sounds. Each SoundResource is a
// representation of a ma_sound type and is responsible for initialization,
// playing, and destruction.
class SoundResource {
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
   private:
    std::unique_ptr<ma_sound> _sound;
    bool _initialized = false;
    std::atomic<bool> _finished{true};
    /*-----------------------------------------------------------------------*/

    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
   private:
    static void onSoundEnd(void* pUserData, ma_sound* pSound);
    void moveFrom(SoundResource& other) noexcept;

   public:
    SoundResource() = default;
    SoundResource(
        ma_engine* engine,
        const char* filePath,
        ma_uint32 flags = MA_SOUND_FLAG_DECODE,
        ma_sound_group* group = nullptr);

    static SoundResource cloneFrom(
        ma_engine* engine,
        SoundResource& templateSound,
        ma_uint32 flags = MA_SOUND_FLAG_DECODE,
        ma_sound_group* group = nullptr);

    ~SoundResource();

    SoundResource(const SoundResource&) = delete;
    SoundResource& operator=(const SoundResource&) = delete;

    SoundResource(SoundResource&& other) noexcept;
    SoundResource& operator=(SoundResource&& other) noexcept;

    void reset();
    bool isFinished() const;
    void updateFinishedState();
    bool playFromStart();
    bool isValid() const;
    void stop();
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== AUDIOPLAYER ======================================================*/
class SoundEffect {
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
   private:
    std::vector<SoundResource> _pool;
    std::string _name;
    /*-----------------------------------------------------------------------*/

    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
   public:
    SoundEffect(std::string name, ma_engine* engine, const std::string& filePath, std::size_t poolSize);
    ~SoundEffect() = default;

    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;
    SoundEffect(SoundEffect&&) noexcept = default;
    SoundEffect& operator=(SoundEffect&&) noexcept = default;

    // void play();
    // void play_callback(void (*callback)());

    bool playFromStart(ma_engine* engine);
    // void playFromStart_callback(void (*callback)());

    void stopAll();
    // void pauseAll();

    const std::string& name() const;
    /*-------- OPERATOR OVERLOADS -------------------------------------------*/
    bool operator==(SoundEffect&) const;
    bool operator<(SoundEffect&) const;
    bool operator>(SoundEffect&) const;
    bool operator<=(SoundEffect&) const;
    bool operator>=(SoundEffect&) const;
    bool operator!=(SoundEffect&) const;
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
