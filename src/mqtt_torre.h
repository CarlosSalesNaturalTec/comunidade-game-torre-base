#pragma once
#include <Arduino.h>

// =====================================================
// MÓDULO MQTT — Comunicação com o Broker
//
// A Torre ASSINA dois tópicos:
//   game/nexus/comando      → recebe START
//   game/defensor/punicao   → recebe DANO_DUPLO
//
// A Torre PUBLICA em um tópico:
//   game/torre/hp           → HP atual após cada hit
// =====================================================

void mqtt_iniciar();
void mqtt_reconectar();
bool mqtt_conectado();
void mqtt_processar();   // chame em TODO loop()

void mqtt_publicarHP(int hp);
