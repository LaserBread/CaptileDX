/* AUDIOPLAYER (Source) - Ethan Jones - audioplayer
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include <string>
#include <vector>

#include "sound.hpp"

/*===========================================================================*/

/*======== SOUNDEFFECT: CONSTRUCTOR =========================================*/
// @param name The name of the sound effect to be used when referencing this
// `SoundEffect`
// @param engine The `AudioEngine` each `SoundResource` should bind to
// @param filePath The file to load. Supports WAV, FLAC, and MP3
// @param poolSize The number of instances to be loaded to support multiple
// copies of this sound to play and overlap.
SoundEffect::SoundEffect(
    std::string name,
    ma_engine* engine,
    const std::string& filePath,
    std::size_t poolSize)
    : _name(std::move(name)) {

    /*-------- VALIDATE INPUTS ----------------------------------------------*/
    if (!engine || poolSize == 0) {
        return;
    }
    /*-----------------------------------------------------------------------*/

    _pool.reserve(poolSize);

    /*-------- LOAD THE ORIGINAL INSTANCE -----------------------------------*/
    SoundResource primary(engine, filePath.c_str());
    if (!primary.isValid()) {
        return;
    }
    /*-----------------------------------------------------------------------*/

    /*-------- CLONE THE REST OF THE INSTANCES ------------------------------*/
    _pool.push_back(std::move(primary));
    for (std::size_t i = 1; i < poolSize; ++i) {
        SoundResource clone = SoundResource::cloneFrom(engine, _pool.front());
        if (clone.isValid()) {
            _pool.push_back(std::move(clone));
        }
    }
    /*-----------------------------------------------------------------------*/
}
/*===========================================================================*/

/*======== SOUNDEFFECT: PLAY FROM START =====================================*/
// Starts a SoundResource (if available) to play the sound.
// @returns `true` if a `SoundResource` was found and able to start playing,
// `false` if all `SoundResources` are busy or the one we chose failed to start
bool SoundEffect::playFromStart(ma_engine* engine){
    for(int i = 0; i < _pool.size(); i++){
        if(_pool[i].isFinished()){
            _pool[i].playFromStart();
            return true;
        }
    }
    return false;
}
/*===========================================================================*/

/*======== SOUNDEFFECT: STOP ALL SOUNDS =====================================*/
// Stops all sounds from every `SoundResource`
void SoundEffect::stopAll(){
    for(int i = 0; i < _pool.size(); i++){
        _pool[i].stop();
    }
}
/*===========================================================================*/

/*======== SOUNDEFFECT: COMPARISON OPERATORS ================================*/
//
const std::string& SoundEffect::name() const {
    return _name;
}

bool SoundEffect::operator==(SoundEffect& that) const {
    return _name == that._name;
}

bool SoundEffect::operator!=(SoundEffect& that) const {
    return _name != that._name;
}

bool SoundEffect::operator>(SoundEffect& that) const {
    return _name > that._name;
}

bool SoundEffect::operator<(SoundEffect& that) const {
    return _name < that._name;
}

bool SoundEffect::operator<=(SoundEffect& that) const {
    return _name <= that._name;
}

bool SoundEffect::operator>=(SoundEffect& that) const {
    return _name >= that._name;
}
/*===========================================================================*/
