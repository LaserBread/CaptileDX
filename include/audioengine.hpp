/* AUDIOPLAYER (Header) - Ethan Jones - audioplayer
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <string>
#include <vector>

#include "miniaudio.h"
/*===========================================================================*/

/*======== FORWARD CLASS DECLARATIONS =======================================*/
class SoundEffect;
/*===========================================================================*/

/*======= AUDIOPLAYER =====================================================*/
class AudioEngine {
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
   private:
    ma_engine _engine;
    std::vector<SoundEffect> _sounds;
    bool _initialized;
    /*-----------------------------------------------------------------------*/

    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
   public:
    AudioEngine();
    ~AudioEngine();

    bool registerEffect(std::string name, const std::string& path, std::size_t poolSize);
    bool unregisterEffect(const std::string& name);

    bool playEffect(const std::string& name);
    bool stopEffect(const std::string& name);
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
