#pragma once

// =====================================================
// ARQUIVO DE CONFIGURAÇÃO — TORRE
// Ajuste aqui sem precisar mexer no resto do código!
// =====================================================

// ── Rede Wi-Fi ───────────────────────────────────────
#define SSID_DA_REDE        "GAME_NEXUS"
#define SENHA_DA_REDE       ""

// ── Broker MQTT ───────────────────────────────────────
#define IP_DO_BROKER        "192.168.1.100"
#define PORTA_MQTT          1883
#define ID_MQTT             "torre_01"

// ── Tópicos MQTT ─────────────────────────────────────
#define TOPICO_HP_TORRE     "game/torre/hp"
#define TOPICO_PUNICAO      "game/defensor/punicao"   // ← Torre assina este
#define TOPICO_COMANDO      "game/nexus/comando"       // ← Torre assina este

// ── Pinos do Hardware ─────────────────────────────────
//    NodeMCU v2 — veja o diagrama completo em WIRING.md
#define PINO_LEDS           D2   // GPIO4  — sinal da fita WS2812B (via resistor 330Ω)
#define PINO_LDR_1          D5   // GPIO14 — pino DO do Módulo LDR 1
#define PINO_LDR_2          D6   // GPIO12 — pino DO do Módulo LDR 2

// DFPlayer Mini usa SoftwareSerial (não bloqueia o loop principal)
#define PINO_DFPLAYER_RX    D7   // GPIO13 — RX do NodeMCU → TX do DFPlayer
#define PINO_DFPLAYER_TX    D8   // GPIO15 — TX do NodeMCU → RX do DFPlayer

// ── Calibração dos Sensores LDR ──────────────────────
//    Aponte o laser no sensor e observe o Serial Monitor.
//    Se "HIT" aparecer sem laser → mude para LOW.
//    Se nada aparecer com laser  → mude para HIGH.
#define LDR_NIVEL_HIT       HIGH

// ── Fita WS2812B 30 LEDs ─────────────────────────────
#define TOTAL_LEDS          30
#define BRILHO_LEDS         90   // 0–255 (90 ≈ 35% — vibrante em eventos)

// ── Parâmetros de Jogo ────────────────────────────────
#define HP_MAXIMO           30
#define DANO_POR_TIRO       3
#define DANO_DUPLO_FATOR    2
#define DEBOUNCE_HIT_MS     400

// ── Modo de Teste (hardware sem rede) ─────────────────
//    Descomente para testar LEDs, LDRs e DFPlayer sem Wi-Fi nem START.
//    LEMBRE de comentar novamente antes do evento!
//
// #define MODO_TESTE

// ── Índices dos sons no cartão SD do DFPlayer ────────
//    Arquivos devem se chamar: 0001.mp3, 0002.mp3...
//    dentro de uma pasta chamada exatamente "01" no SD.
#define SOM_INICIO          1    // "Batalha Iniciada! Protejam a Base."
#define SOM_IMPACTO         2    // explosão curta ao receber tiro
#define SOM_DANO_DUPLO      3    // explosão forte ao receber punição
#define SOM_GAME_OVER       4    // torre destruída
