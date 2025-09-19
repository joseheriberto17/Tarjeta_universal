# SSD1306 – UML del flujo actual (Mermaid)
**Autor:** jose heriberto  
**Fecha:** 2025-09-14  
**Propósito:** Documentar el flujo real del proyecto (init, armado de buffers, envío I²C, texto/figuras y scroll).  
**Ámbito:** App (main.c), Driver SSD1306 (cmd/ensamble), HAL/ASF (TWI0, PIO, PMC, SysTick).  
**Refs:** `SSD1306_cmd.c`, `SSD1306.h`, `main.c`

---

## 1) Vista de componentes (capas y dependencias)
```mermaid
flowchart TB
  subgraph App["App (main.c)"]
    Minit["sysclk_init/board_init\nconfigure_systick()\nconfigure_pins()"]
    Mdraw["Secuencia de dibujo:\nInicialize_SSD1306_default()\nNew_page()/frase_*/form_hearts()/vaciados()\nscroll() -> ssd1306_write()"]
    UART["UART_Handler (ISR)\n- lee RHR, limpia errores\n- eco debug"]
  end

  subgraph Driver["Driver SSD1306 (lógica de comandos)"]
    CMD["SSD1306_cmd.c\n(Set_*, Activate/Deactivate_Scroll,\nSet_Page_Start_..., etc.)"]
    BUF["Ensamble de buffer TWI\n(0x80 cmd-cont, 0x00 cmd-last,\n0xC0 data-cont, 0x40 data-last)"]
  end

  subgraph HAL["HAL / ASF (ATSAM3X8E)"]
    TWI["TWI0 (twi_master_init/twi_master_write)\nconfigure_pins_TWI_SSD1306()"]
    PIO["PIO (LEDs, mapeo pines)"]
    PMC["PMC clocks"]
    SysTick["SysTick (millis)"]
  end

  App --> Driver
  Driver --> HAL
  UART --> App
  Mdraw --> CMD
  CMD --> BUF
  Mdraw --> TWI
  BUF --> TWI
  Minit --> PMC
  Minit --> PIO
  Minit --> SysTick
```

---

## 2) Secuencia – Arranque e inicialización
```mermaid
sequenceDiagram
  autonumber
  participant Main as main.c
  participant HAL as HAL/ASF (PMC/PIO/SysTick)
  participant TWIc as configure_pins_TWI_SSD1306()
  participant CMD as Inicialize_SSD1306_default()
  participant BUF as Armado de buffer (0x80/0x00)
  participant I2C as ssd1306_write() / TWI0

  Main->>HAL: sysclk_init(), board_init()
  Main->>HAL: configure_systick()
  Main->>HAL: configure_pins() (LEDs)
  Main->>TWIc: configurar TWI0 (100 kHz) + pines PA17/PA18
  Main->>CMD: Inicialize_SSD1306_default(buffer_TWI, &len)
  activate CMD
  CMD->>BUF: Set_* cmds (AE, A8, D3, 40, A1, C8, DA, 81, A4/A5, A6/A7, D5, 8D, AF)
  BUF-->>CMD: buffer_TWI con parejas [0x80|0x00, cmd]
  deactivate CMD
  Main->>I2C: ssd1306_write(buffer_TWI, len*2)
  I2C-->>Main: status
```

---

## 3) Secuencia – Renderizado por páginas (texto/figuras/limpios y scroll)
```mermaid
sequenceDiagram
  autonumber
  participant Main as main.c
  participant CMD as New_page()/frase_A/B/C()/form_hearts()/vaciados()/scroll()
  participant BUF as Armado de buffer (0x80 cmd / 0xC0 data)
  participant I2C as ssd1306_write()/TWI0

  Main->>CMD: New_page(..., PAGE_0)
  CMD->>BUF: [0x80, B0|page] ... ultimo con 0x00
  BUF-->>Main: buffer_TWI, len
  Main->>I2C: ssd1306_write()
  end

  Main->>CMD: form_hearts()
  CMD->>BUF: datos con 0xC0 (cont.) y 0x40 (ultimo)
  BUF-->>Main: buffer_TWI (len=256)
  Main->>I2C: ssd1306_write(buffer_TWI, len)


  Note over Main: Vaciar paginas 1,2,6
  loop por cada pagina {1,2,6}
    Main->>CMD: New_page(PAGE_n)
    CMD->>BUF: comando B0|page (0x80...0x00)
    Main->>I2C: ssd1306_write(cmd)

    Main->>CMD: vaciados() (datos 0x00)
    CMD->>BUF: datos 0xC0...0x40
    Main->>I2C: ssd1306_write(data)
  end
  end


  Note over Main: Frases en paginas 3,4,5
  loop frase_A/B/C
    Main->>CMD: New_page(PAGE_k)
    CMD->>BUF: B0|page (0x80...0x00)
    Main->>I2C: ssd1306_write(cmd)

    Main->>CMD: frase_X() -> memcpy glyphs (128B)
    CMD->>BUF: datos 0xC0...0x40
    Main->>I2C: ssd1306_write(data)
  end
  end

  Note over Main: Scroll horizontal
  Main->>CMD
  CMD->>BUF: 0x26/0x27 + parametros; 0x2F (activate)
  end
  BUF-->>Main: buffer_TWI cmds (0x80...0x00)
  Main->>I2C: ssd1306_write(cmds scroll)
```

---

## 4) Máquina de estados – Driver/flujo SSD1306
```mermaid
stateDiagram-v2
  [*] --> Reset

  state Reset {
    note right: HW/SW reset
  }

  Reset --> InitSequence: Inicialize_SSD1306_default()
  InitSequence --> Ready: AF (Display ON) OK
  InitSequence --> Error: NAK / timeout

  Ready --> Updating: New_page()/frase_*/form_hearts()/vaciados()\n-> ssd1306_write(data)
  Updating --> Ready: última página/último byte (0x40) OK
  Updating --> Error: NAK / write fail

  Ready --> Scrolling: scroll() -> (0x26/0x27 ... 0x2F)
  Scrolling --> Ready: Deactivate_Scroll (0x2E) o redraw
  Scrolling --> Error: NAK

  Error --> Reset: política de reintento (max N)
```

---

## 5) Actividad – Ensamblado de buffer TWI (comandos y datos)
```mermaid
flowchart LR
  A[Iniciar buffer_TWI] --> B{¿Tipo?}
  B -- Comando --> C[Escribir 0x80 (continuación)\npara todos menos el último]
  C --> D[Escribir CMD]
  D --> E{¿Último comando del bloque?}
  E -- Sí --> F[Usar 0x00 en el último CMD]
  E -- No --> B

  B -- Datos --> G[Escribir 0xC0 (continuación)\npara todos menos el último]
  G --> H[Escribir DATA]
  H --> I{¿Último dato del bloque?}
  I -- Sí --> J[Usar 0x40 en el último DATA]
  I -- No --> B
```
