#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H


#include <QMediaPlayer>

#include "Globals.h"
#include "Move.h"

class SoundPlayer{
public:
    static void playSound(Move* move, GameState state){
        static QMediaPlayer* player;
        if(player == nullptr)
            player = new QMediaPlayer();
        if(state == GameState::CHECKMATE || state == GameState::STALEMATE)
            player->setMedia(QUrl("qrc:/wavs/Resources/sound_standard_CheckMate.wav"));
        else if(state == GameState::CHECK)
            player->setMedia(QUrl("qrc:/wavs/Resources/sound_standard_Check.wav"));
        else if(move->getMoveType() == MoveType::CASTLING)
            player->setMedia(QUrl("qrc:/wavs/Resources/sound_standard_Castling.wav"));
        else if(move->getCapturedPiece() != nullptr)
            player->setMedia(QUrl("qrc:/wavs/Resources/sound_standard_Capture.wav"));
        else player->setMedia(QUrl("qrc:/wavs/Resources/sound_standard_Move.wav"));

        player->play();
    }
private:
    SoundPlayer();
};

#endif // SOUNDPLAYER_H
