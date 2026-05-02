#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "config.h"

// =====================================================
// MÓDULO DE ÁUDIO — DFPlayer Mini
//
// O DFPlayer Mini é um pequeno reprodutor de MP3.
// Ele lê arquivos de um cartão microSD e toca via
// saída de áudio (conectado a uma caixa de som).
//
// ESTRUTURA OBRIGATÓRIA DO CARTÃO SD:
//   SD:/
//   └── 01/
//       ├── 0001.mp3  ← "Batalha Iniciada! Protejam a Base."
//       ├── 0002.mp3  ← som de impacto (tiro direto)
//       ├── 0003.mp3  ← explosão forte (DANO DUPLO)
//       └── 0004.mp3  ← "Torre destruída!" (Game Over)
//
// IMPORTANTE: nome de pasta e arquivo EXATAMENTE assim.
//             O DFPlayer não aceita variações.
// =====================================================

bool dfplayer_iniciar();          // retorna false se o DFPlayer não responder

void dfplayer_tocarInicio();      // voz épica de início de batalha
void dfplayer_tocarImpacto();     // som curto de tiro/impacto
void dfplayer_tocarDanoDuplo();   // explosão mais intensa (punição)
void dfplayer_tocarGameOver();    // narração de Game Over
