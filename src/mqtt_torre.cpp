#include "mqtt_torre.h"
#include "jogo_torre.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

static WiFiClient   _wifi;
static PubSubClient _mqtt(_wifi);

// ─────────────────────────────────────────────────────
// CALLBACK: roteador de mensagens recebidas
// A Torre escuta dois tópicos diferentes aqui
// ─────────────────────────────────────────────────────
static void aoReceberMensagem(char* topico, byte* payload, unsigned int tamanho) {
  char mensagem[tamanho + 1];
  memcpy(mensagem, payload, tamanho);
  mensagem[tamanho] = '\0';

  Serial.print("[MQTT] Mensagem em '");
  Serial.print(topico);
  Serial.print("': ");
  Serial.println(mensagem);

  // ── Rota 1: comando do Nexus ──────────────────────
  if (String(topico) == TOPICO_COMANDO) {
    StaticJsonDocument<64> doc;
    if (deserializeJson(doc, mensagem) != DeserializationError::Ok) return;

    const char* cmd = doc["cmd"] | "";
    if (strcmp(cmd, "START") == 0) {
      Serial.println("[MQTT] → Comando START recebido!");
      jogo_iniciarPartida();
    }

  // ── Rota 2: punição do Defensor ───────────────────
  } else if (String(topico) == TOPICO_PUNICAO) {
    StaticJsonDocument<64> doc;
    if (deserializeJson(doc, mensagem) != DeserializationError::Ok) return;

    const char* tipo = doc["tipo"] | "";
    if (strcmp(tipo, "DANO_DUPLO") == 0) {
      Serial.println("[MQTT] → Punição DANO_DUPLO recebida!");
      jogo_aplicarDanoDuplo();
    }
  }
}

// ─────────────────────────────────────────────────────
void mqtt_iniciar() {
  // ── 1. Configurar broker e callback (independe do Wi-Fi) ──
  _mqtt.setServer(IP_DO_BROKER, PORTA_MQTT);
  _mqtt.setCallback(aoReceberMensagem);

  // ── 2. Tentar conectar ao Wi-Fi com timeout ────────
  //    Se a rede não estiver disponível em 10s, a Torre
  //    entra em MODO OFFLINE: LEDs e LDRs funcionam
  //    normalmente. O loop() tentará reconectar depois.
  Serial.print("[MQTT] Conectando ao Wi-Fi '");
  Serial.print(SSID_DA_REDE);
  Serial.print("' (timeout: 10s)");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_DA_REDE, SENHA_DA_REDE);

  constexpr unsigned long TIMEOUT_WIFI_MS = 10000;
  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - inicio >= TIMEOUT_WIFI_MS) {
      Serial.println();
      Serial.println("[MQTT] Wi-Fi nao encontrado — MODO OFFLINE ativado.");
      Serial.println("[MQTT] A Torre funciona normalmente.");
      Serial.println("[MQTT] Reconexao automatica sera tentada no loop().");
      return;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.print(" OK! IP: ");
  Serial.println(WiFi.localIP());

  // ── 3. Conectar ao broker MQTT ────────────────────
  mqtt_reconectar();
}

void mqtt_reconectar() {
  if (_mqtt.connected()) return;

  // ── Sem Wi-Fi → tenta reconectar a rede primeiro ──
  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long _ultimaTentativaWifi = 0;
    if (millis() - _ultimaTentativaWifi < 5000) return;
    _ultimaTentativaWifi = millis();

    Serial.println("[MQTT] Wi-Fi desconectado. Tentando reconectar...");
    WiFi.disconnect();
    WiFi.begin(SSID_DA_REDE, SENHA_DA_REDE);
    return;
  }

  // ── Wi-Fi OK → tenta o broker MQTT ───────────────
  Serial.print("[MQTT] Conectando ao broker...");

  if (_mqtt.connect(ID_MQTT)) {
    Serial.println(" Conectado!");

    // A Torre assina DOIS tópicos ao conectar
    _mqtt.subscribe(TOPICO_COMANDO);
    _mqtt.subscribe(TOPICO_PUNICAO);

    Serial.print("[MQTT] Assinando: ");
    Serial.println(TOPICO_COMANDO);
    Serial.print("[MQTT] Assinando: ");
    Serial.println(TOPICO_PUNICAO);

    // Republicar HP atual ao reconectar
    mqtt_publicarHP(jogo_getHP());

  } else {
    Serial.print(" Falhou! Codigo MQTT: ");
    Serial.println(_mqtt.state());
  }
}

bool mqtt_conectado() {
  return _mqtt.connected();
}

void mqtt_processar() {
  _mqtt.loop();
}

// ─────────────────────────────────────────────────────
void mqtt_publicarHP(int hp) {
  // Monta: {"hp": 24}
  StaticJsonDocument<48> doc;
  doc["hp"] = hp;

  char payload[48];
  serializeJson(doc, payload);

  _mqtt.publish(TOPICO_HP_TORRE, payload);

  Serial.print("[MQTT] Publicou HP da Torre: ");
  Serial.println(payload);
}
