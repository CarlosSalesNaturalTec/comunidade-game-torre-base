# Ligação Elétrica — Torre

## Diagrama de Conexões

```
NodeMCU v2              Fita WS2812B IP67 (30 LEDs)
─────────────           ────────────────────────────
Vin (5V)  ──┬────────── +5V
             │
            [1000µF]   ← capacitor entre Vin e GND
             │          (protege a fita — obrigatório com 30 LEDs)
GND       ──┴────────── GND
D2        ──[330Ω]───── DI   ← resistor de 330Ω em série (proteção de dados)

NodeMCU v2              Módulo LDR 1
─────────────           ─────────────
3V3       ──────────── VCC
GND       ──────────── GND
D5        ──────────── DO  (saída digital)

NodeMCU v2              Módulo LDR 2
─────────────           ─────────────
3V3       ──────────── VCC
GND       ──────────── GND
D6        ──────────── DO  (saída digital)

NodeMCU v2              DFPlayer Mini
─────────────           ────────────────────────────────────────
Vin (5V)  ──────────── VCC   ← DFPlayer precisa de 5V (não 3.3V)
GND       ──────────── GND
D7        ──[1kΩ]───── RX    ← resistor de 1kΩ entre D7 e o pino RX
D8        ──────────── TX    ← sem resistor neste lado

DFPlayer Mini           Caixa de Som (alto-falante passivo)
─────────────           ─────────────────────────────────────
SPK_1     ──────────── (+) da caixa
SPK_2     ──────────── (−) da caixa
```

---

## Tabela Resumo de Pinos

| Componente          | Pino NodeMCU | GPIO    | Observação                     |
|---------------------|-------------|---------|--------------------------------|
| Fita WS2812B DI     | D2          | GPIO4   | via resistor 330Ω              |
| Fita WS2812B VCC    | Vin (5V)    | —       | via capacitor 1000µF / GND     |
| Módulo LDR 1 DO     | D5          | GPIO14  | —                              |
| Módulo LDR 2 DO     | D6          | GPIO12  | —                              |
| DFPlayer RX         | D7          | GPIO13  | via resistor 1kΩ               |
| DFPlayer TX         | D8          | GPIO15  | sem resistor                   |
| DFPlayer VCC        | Vin (5V)    | —       | —                              |

### Componentes adicionais obrigatórios

| Componente    | Valor        | Posição                                   |
|---------------|--------------|-------------------------------------------|
| Resistor      | 330 Ω        | entre D2 e DI da fita                     |
| Capacitor     | 1000 µF / 6V | entre Vin e GND, próximo à entrada da fita|
| Resistor      | 1 kΩ         | entre D7 (NodeMCU) e RX (DFPlayer)        |

---

## Atenção: conflito de boot no D8 (GPIO15)

O NodeMCU ESP8266 usa o GPIO15 (D8) durante o boot para
determinar o modo de inicialização.

**Problema:** se o DFPlayer tiver o pino TX ativo (HIGH) no momento
do boot, o NodeMCU pode falhar ao inicializar.

**Solução:** conecte um resistor de 10kΩ entre D8 e GND (pull-down).
Isso garante que o GPIO15 inicie LOW durante o boot e sobe para
o nível correto depois que o DFPlayer inicializar.

```
D8 (GPIO15) ──────── TX do DFPlayer
     │
    [10kΩ]     ← pull-down de boot
     │
    GND
```

---

## Preparação do Cartão SD para o DFPlayer Mini

O DFPlayer Mini é muito exigente com a estrutura de arquivos.
Siga exatamente:

### Passo a passo

1. **Formatar o cartão SD.**
   Use um cartão de até 32 GB.
   Formate em FAT32 (não exFAT, não NTFS).
   No Windows: botão direito no SD → Formatar → FAT32.

2. **Criar a pasta "01".**
   Na raiz do SD, crie uma pasta com o nome exato `01`
   (dois caracteres: zero e um).

3. **Copiar os arquivos de áudio.**
   Dentro da pasta `01`, coloque os arquivos MP3:
   ```
   SD:/
   └── 01/
       ├── 0001.mp3   → narração "Batalha Iniciada! Protejam a Base."
       ├── 0002.mp3   → som de impacto / tiro
       ├── 0003.mp3   → explosão forte (Dano Duplo)
       └── 0004.mp3   → narração "Torre Destruída!" (Game Over)
   ```
   Os nomes devem ter exatamente 4 dígitos + extensão .mp3.
   Qualquer variação (0001.MP3, 1.mp3, som1.mp3) causa silêncio sem erro.

4. **Ordem de cópia importa.**
   Copie os arquivos UM POR UM, em ordem (0001 primeiro, depois 0002...).
   O DFPlayer indexa pela ordem de escrita no FAT, não pelo nome.

5. **Verificar no Serial Monitor.**
   Ao ligar, se aparecer "[DFPLAYER] ERRO: DFPlayer nao respondeu!",
   as causas mais comuns são:
   - Cartão SD não inserido ou mal inserido
   - Arquivos fora da pasta "01"
   - Resistor de 1kΩ faltando entre D7 e o RX do DFPlayer
   - DFPlayer alimentado com 3.3V (precisa de 5V)

---

## Disposição Circular dos 30 LEDs

A fita deve ser dobrada ou colada formando um anel ao redor
da base da Torre (estrutura de EVA, PVC ou madeira).

```
         [LED 14] [LED 15] [LED 16]
      [13]                          [17]
   [12]                                [18]
  [11]                                  [19]
 [10]         TORRE / BASE              [20]
  [9]         (LDRs apontam             [21]
   [8]         para fora)              [22]
      [7]                          [23]
         [6]  [5]  [4]  [3]  [2]  [1→24..30]
```

Posicione os dois módulos LDR opostos entre si no anel,
com os sensores apontando para fora (na direção do campo de jogo).
Isso garante cobertura de hits vindos de ângulos diferentes.
