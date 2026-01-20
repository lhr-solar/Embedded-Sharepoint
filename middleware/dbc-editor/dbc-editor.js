(async () => {
  "use strict";

  /**
   * CONSTANTS
   */

  /**
   * A map to determine endianess.
   */
  const ENDIAN_MAP = {
    intel: "little",
    motorola: "big"
  };

  /**
   * Useful column indexes.
   */
  const MESSAGE_NAME = 2;
  const MESSAGE_CAN_ID = 3;
  const MESSAGE_TYPE = 4;
  const MESSAGE_DLC = 5;
  const MESSAGE_COMMENT = 6;
  const SIGNAL_NAME = 2;
  const SIGNAL_TYPE = 3;
  const SIGNAL_ORDER = 4;
  const SIGNAL_MODE = 5;
  const SIGNAL_MULTIPLEXOR = 6;
  const SIGNAL_MULTIPLEX_VALUE = 7;
  const SIGNAL_START = 8;
  const SIGNAL_LENGTH = 9;
  const SIGNAL_FACTOR = 10;
  const SIGNAL_OFFSET = 11;
  const SIGNAL_MIN = 12;
  const SIGNAL_MAX = 13;
  const SIGNAL_UNIT = 14;
  const SIGNAL_COMMENT = 15;
  const FRAME_DATA_HEX = 1;
  const FRAME_DATA_BIN = 2;

  /**
   * ELEMENT REFERENCES
   */

  /** @type {HTMLDivElement} */
  const modalBackdrop = document.querySelector("#dbc-modal-backdrop");

  /** @type {HTMLDivElement} */
  const modalClose = document.querySelector("#dbc-modal-close");

  /** @type {HTMLDivElement} */
  const modalContent = document.querySelector("#dbc-modal-content");

  /** @type {HTMLDivElement} */
  const modalActions = document.querySelector("#dbc-modal-actions");

  /** @type {HTMLButtonElement} */
  const openButton = document.querySelector("#dbc-open-button");

  /** @type {HTMLButtonElement} */
  const downloadButton = document.querySelector("#dbc-download-button");

  /** @type {HTMLInputElement} */
  const fileInput = document.querySelector("#dbc-file-input");

  /** @type {HTMLSelectElement} */
  const numberFormatSelect = document.querySelector(
    "#dbc-number-format-select"
  );

  /** @type {HTMLElement} */
  const canMessagesBody = document.querySelector("#dbc-can-messages-body");

  /** @type {HTMLElement} */
  const canSignalsBody = document.querySelector("#dbc-can-signals-body");

  /** @type {HTMLElement} */
  const bitIndicesBody = document.querySelector("#dbc-bit-indices-body");

  /** @type {HTMLElement} */
  const canFrameBody = document.querySelector("#dbc-can-frame-body");

  /** @type {HTMLElement} */
  const signalMaskBody = document.querySelector("#dbc-signal-mask-body");

  /** @type {HTMLSpanElement} */
  const resultDataBin = document.querySelector("#dbc-result-data-bin");

  /** @type {HTMLSpanElement} */
  const resultDataHex = document.querySelector("#dbc-result-data-hex");

  /** @type {HTMLSpanElement} */
  const resultDataDec = document.querySelector("#dbc-result-data-dec");

  /** @type {HTMLSpanElement} */
  const resultValueFactor = document.querySelector("#dbc-result-value-factor");

  /** @type {HTMLSpanElement} */
  const resultValueDec = document.querySelector("#dbc-result-value-dec");

  /** @type {HTMLSpanElement} */
  const resultValueOffset = document.querySelector("#dbc-result-value-offset");

  /** @type {HTMLSpanElement} */
  const resultValue = document.querySelector("#dbc-result-value");

  /** @type {HTMLSpanElement} */
  const resultValueUnit = document.querySelector("#dbc-result-value-unit");

  /** @type {HTMLPreElement} */
  const preview = document.querySelector("#dbc-preview");

  /**
   * VARIABLES
   */

  /** @type {'hex' | 'dec'} */
  let prevNumberFormat = "dec";

  /** @type {'hex' | 'dec'} */
  let currNumberFormat = "hex";

  /** @type {Record<string, TableParams>} */
  let tableParams = {};

  /** @type {Record<string, CanMessage>} */
  let canMessages = {};

  /** @type {unknown[][]} */
  let bitIndices = [];
  let bitIndices1 = [];

  /** @type {unknown[][]} */
  const canFrame = [];

  let isFirstCanFrameInsertion = true;

  let filename = "";

  let file = `VERSION ""


NS_ : 
		NS_DESC_
		CM_
		BA_DEF_
		BA_
		VAL_
		CAT_DEF_
		CAT_
		FILTER
		BA_DEF_DEF_
		EV_DATA_
		ENVVAR_DATA_
		SGTYPE_
		SGTYPE_VAL_
		BA_DEF_SGTYPE_
		BA_SGTYPE_
		SIG_TYPE_REF_
		VAL_TABLE_
		SIG_GROUP_
		SIG_VALTYPE_
		SIGTYPE_VALTYPE_
		BO_TX_BU_
		BA_DEF_REL_
		BA_REL_
		BA_DEF_DEF_REL_
		BU_SG_REL_
		BU_EV_REL_
		BU_BO_REL_
		SG_MUL_VAL_

BS_:

BU_:


BO_ 2364540158 EEC1: 8 Vector__XXX
	SG_ EngineSpeed : 24|16@1+ (0.125,0) [0|8031.875] "rpm" Vector__XXX

BO_ 2566844926 CCVS1: 8 Vector__XXX
	SG_ WheelBasedVehicleSpeed : 8|16@1+ (0.00390625,0) [0|250.996] "km/h" Vector__XXX


CM_ BO_ 2364540158 "Electronic Engine Controller 1";
CM_ SG_ 2364540158 EngineSpeed "Actual engine speed which is calculated over a minimum crankshaft angle of 720 degrees divided by the number of cylinders.…";
CM_ BO_ 2566844926 "Cruise Control/Vehicle Speed 1";
CM_ SG_ 2566844926 WheelBasedVehicleSpeed "Wheel-Based Vehicle Speed: Speed of the vehicle as calculated from wheel or tailshaft speed.";
BA_DEF_ SG_  "SPN" INT 0 524287;
BA_DEF_ BO_  "VFrameFormat" ENUM  "StandardCAN","ExtendedCAN","reserved","J1939PG";
BA_DEF_  "DatabaseVersion" STRING ;
BA_DEF_  "BusType" STRING ;
BA_DEF_  "ProtocolType" STRING ;
BA_DEF_  "DatabaseCompiler" STRING ;
BA_DEF_DEF_  "SPN" 0;
BA_DEF_DEF_  "VFrameFormat" "J1939PG";
BA_DEF_DEF_  "DatabaseVersion" "DEMO PLUS";
BA_DEF_DEF_  "BusType" "";
BA_DEF_DEF_  "ProtocolType" "";
BA_DEF_DEF_  "DatabaseCompiler" "";
BA_ "ProtocolType" "J1939";
BA_ "BusType" "CAN";
BA_ "DatabaseCompiler" "CSS ELECTRONICS (WWW.CSSELECTRONICS.COM)";
BA_ "DatabaseVersion" "1.0.0";
BA_ "VFrameFormat" BO_ 2364540158 3;
BA_ "VFrameFormat" BO_ 2566844926 3;
BA_ "SPN" SG_ 2364540158 EngineSpeed 190;
BA_ "SPN" SG_ 2566844926 WheelBasedVehicleSpeed 84;`;

  /** @type {Line[]} */
  let lines = [];

  /** @type {Line} */
  let protocolLine = {
    type: "protocol",
    rawLine: "",
    values: {}
  };

  /** @type {Line[]} */
  let newMessagesLines = [];

  /** @type {Line[]} */
  let newCommentsLines = [];

  /** @type {Line[]} */
  let newTypesLines = [];

  /** @type {Line[]} */
  let newMultiplexLines = [];

  let lineSeparator = "\n";
  let signalIndentation = "  ";

  /**
   * FUNCTIONS
   */

  const openFileDialog = () => {
    //console.debug('Opening file dialog');

    fileInput.dispatchEvent(new MouseEvent("click"));
  };

  const loadFile = () => {
    //console.debug('Loading file');

    const reader = new FileReader();
    reader.onload = () => {
      filename = fileInput.files[0].name;
      file = reader.result;
      parseFile();
    };
    reader.onerror = (err) => console.error(err);
    reader.readAsText(fileInput.files[0], "utf-8");
  };

  const parseFile = () => {
    //console.debug('Parsing file');

    canMessages = {};
    lines = [];
    newMessagesLines = [];
    newCommentsLines = [];
    newTypesLines = [];
    newMultiplexLines = [];
    lineSeparator = file.match(/\r?\n|\r/)[0];

    //console.debug(canMessages, lines, newMessagesLines, newCommentsLines, newMultiplexLines);

    const rawLines = file.split(lineSeparator);
    const messagesByCanId = {};
    let i = 0;
    let currentMessageCanId = null;
    let wasPrevLineSignal = false;
    let wasPrevLineSignalComment = false;
    let lastMessageLine = 0;
    let lastCommentLine = 0;
    let lastMultiplexLine = 0;
    let lastTypeLine = 0;
    
    // Add a flag to track if we've found the VFrameFormat attribute definition
    let hasVFrameFormat = false;
    // Track the index of the BS_: line
    let bsLineIndex = -1;
    // Track the index of the first BA_DEF_ line after BS_: line
    let firstBaDefAfterBsIndex = -1;

    for (const rawLine of rawLines) {
      const line = { rawLine };
      const trimmedLine = rawLine.trim();
      let matches = null;
      let isMessageLine = false;
      let isSignalLine = false;
      let isMessageCommentLine = false;
      let isSignalCommentLine = false;
      let isSignalMultiplexLine = false;
      let isTypeLine = false;
      
      // Check if this line defines the VFrameFormat attribute
      if (trimmedLine.startsWith('BA_DEF_ BO_  "VFrameFormat"')) {
        hasVFrameFormat = true;
      }
      
      // Track the BS_: line
      if (trimmedLine === 'BS_:') {
        bsLineIndex = i;
      }
      
      // Track the first BA_DEF_ line after the BS_: line
      if (bsLineIndex !== -1 && firstBaDefAfterBsIndex === -1 && trimmedLine.startsWith('BA_DEF_')) {
        firstBaDefAfterBsIndex = (i +2);
      }

      if (!trimmedLine) {
        line.type = "blank";
      } else if (trimmedLine.startsWith("BO_")) {
        // Line is a message

        /**
         * Message format:
         *
         * `BO_ {CAN_ID} {NAME}: {DLC} {SENDER}`
         *
         * - `{CAN_ID}`: decimal value
         * - `{NAME}`: 1-32 characters with letters, digits and underscores
         * - `{DLC}`: integer between 0 and 1785
         * - (optional) `{SENDER}`: string (defaults to "Vector__XXX")
         *
         * Example:
         *
         * `BO_ 2364540158 EEC1: 8 Vector__XXX`
         */
        matches = trimmedLine.match(/BO_\s(\d+)\s(\w+):\s(\d+)(?:\s(.+))?/);

        if (matches) {
          const [, canIdDec, name, dlc, sender = "Vector__XXX"] = matches;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.values) {
            currentMessage.values = [
              ...tableParams["dbc-can-messages-body"].defaultValues
            ];
          }

          currentMessage.values[MESSAGE_NAME] = name;
          currentMessage.values[MESSAGE_CAN_ID] = formatNumber(canIdDec, "dec");
          currentMessage.values[MESSAGE_DLC] = Number.parseInt(dlc);

          line.values = { canIdDec, name, dlc, sender };
          currentMessage.line = line;

          currentMessageCanId = canIdDec;
          isMessageLine = true;
        }
      } else if (trimmedLine.startsWith("SG_MUL_VAL_")) {
        // Line is a multiplex

        /**
         * Multiplex format:
         *
         * `SG_MUL_VAL_ {CAN_ID} {NAME} {MULTIPLEXOR} {MULTIPLEX_VALUE}-{MULTIPLEX_VALUE}`;
         *
         * - `{CAN_ID}`: decimal value
         * - `{NAME}`: 1-32 characters with letters, digits and underscores
         * - `{MULTIPLEXOR}`: 1-32 characters with letters, digits and underscores
         * - `{MULTIPLEX_VALUE}`: decimal value
         *
         * Example:
         *
         * `SG_MUL_VAL_ 2024 S1_PID_0D_VehicleSpeed ParameterID_Service01 13-13;`
         */
        matches = trimmedLine.match(
          /SG_MUL_VAL_\s(\d+)\s(\w+)\s(\w+)\s(\d+)-(\d+);/
        );

        if (matches) {
          const [, canIdDec, name, multiplexor, multiplexValue] = matches;
          const id = name;
          const multiplexorId = multiplexor;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.signals) {
            currentMessage.signals = {};
          }

          if (!currentMessage.signalsOrder) {
            currentMessage.signalsOrder = [];
          }

          if (!currentMessage.selectedSignalId) {
            currentMessage.selectedSignalId = id;
          }

          let currentSignal = currentMessage.signals[id];

          if (!currentSignal) {
            currentSignal = {};
            currentMessage.signals[id] = currentSignal;
            currentMessage.signalsOrder.push(id);
          }

          if (!currentSignal.values) {
            currentSignal.values = [
              ...tableParams["dbc-can-signals-body"].defaultValues
            ];
          }

          currentSignal.values[SIGNAL_MULTIPLEXOR] = multiplexorId;
          currentSignal.values[SIGNAL_MULTIPLEX_VALUE] = Number.parseInt(
            multiplexValue
          );

          line.values = { canIdDec, name, multiplexor, multiplexValue };
          currentSignal.multiplexLine = line;

          isSignalMultiplexLine = true;
        }
      } else if (trimmedLine.startsWith("SG_")) {
        // Line is a signal

        /**
         * Signal format:
         *
         * `SG_ {NAME} [m{MULTIPLEX_VALUE}][M] : {BIT_START}|{BIT_LENGTH}@{ENDIANESS}{SIGNAL_TYPE} ({FACTOR},{OFFSET}) [{MIN}|{MAX}] "{UNIT}" {RECEIVER}`
         *
         * - `{NAME}`: 1-32 characters with letters, digits and underscores
         * - (optional) `{MULTIPLEX_VALUE}`: number
         * - `{BIT_START}`: integer
         * - `{BIT_LENGTH}`: integer
         * - `{ENDIANESS}`: "1" for little endian, "0" for big endian
         * - `{SIGNAL_TYPE}`: "+" for unsigned, "-" for signed
         * - `{FACTOR}`: number
         * - `{OFFSET}`: number
         * - (optional) `{MIN}`: number (defaults to "0")
         * - (optional) `{MAX}`: number (defaults to "0")
         * - (optional) `{UNIT}`: string (defaults to "")
         * - (optional) `{RECEIVER}`: string (defaults to "Vector__XXX")
         *
         * Example:
         *
         * `SG_ EngineSpeed : 24|16@1+ (0.125,0) [0|8031.875] "rpm" Vector__XXX`
         */
        matches = trimmedLine.match(
          /SG_\s(\w+)(\s(m(\d+))?(M)?)?\s?:\s(\d+)\|(\d+)@(0|1)(\+|-)\s\((.+?),(.+?)\)(?:\s\[(.*?)\|(.*?)])?(?:\s"(.*?)")?(?:\s(.+))?/
        );

        if (matches) {
          const [
            ,
            name,
            ,
            ,
            multiplexValue,
            isMultiplexor,
            bitStart,
            bitLength,
            endianess,
            signalType,
            factor,
            offset,
            min = "0",
            max = "0",
            unit = "",
            receiver = "Vector__XXX"
          ] = matches;
          const id = name;

          let currentMessage = messagesByCanId[currentMessageCanId];

          if (!currentMessage.signals) {
            currentMessage.signals = {};
          }

          if (!currentMessage.signalsOrder) {
            currentMessage.signalsOrder = [];
          }

          if (!currentMessage.selectedSignalId) {
            currentMessage.selectedSignalId = id;
          }

          let currentSignal = currentMessage.signals[id];

          if (!currentSignal) {
            currentSignal = {};
            currentMessage.signals[id] = currentSignal;
            currentMessage.signalsOrder.push(id);
          }

          if (!currentSignal.values) {
            currentSignal.values = [
              ...tableParams["dbc-can-signals-body"].defaultValues
            ];
          }

          currentSignal.values[SIGNAL_NAME] = name;
          currentSignal.values[SIGNAL_TYPE] =
            signalType === "+" ? "unsig" : "sig";
          currentSignal.values[SIGNAL_ORDER] =
            endianess === "1" ? "intel" : "motorola";
          currentSignal.values[SIGNAL_MODE] = multiplexValue
            ? "multiplexingGroup"
            : isMultiplexor
              ? "multiplexor"
              : "signal";
          currentSignal.values[SIGNAL_MULTIPLEX_VALUE] = multiplexValue
            ? Number.parseInt(multiplexValue)
            : 0;
          currentSignal.values[SIGNAL_START] = Number.parseInt(bitStart);
          currentSignal.values[SIGNAL_LENGTH] = Number.parseInt(bitLength);
          currentSignal.values[SIGNAL_FACTOR] = Number.parseFloat(factor);
          currentSignal.values[SIGNAL_OFFSET] = Number.parseFloat(offset);
          currentSignal.values[SIGNAL_MIN] = Number.parseFloat(min);
          currentSignal.values[SIGNAL_MAX] = Number.parseFloat(max);
          currentSignal.values[SIGNAL_UNIT] = unit;

          line.values = {
            name,
            bitStart,
            bitLength,
            endianess,
            signalType,
            factor,
            offset,
            min,
            max,
            unit,
            receiver
          };
          currentSignal.line = line;

          wasPrevLineSignal = true;
          isSignalLine = true;

          signalIndentation = rawLine.match(/^(\s|\t)+/)[0];
        }
      } else if (trimmedLine.startsWith("CM_ BO_")) {
        // Line is comment for a message

        /**
         * Comment format:
         *
         * `CM_ BO_ {CAN_ID} "{COMMENT}";`
         *
         * - `{CAN_ID}`: decimal value
         * - `{COMMENT}`: string
         *
         * Example:
         *
         * `CM_ BO_ 2364540158 "Electronic Engine Controller 1";`
         */
        matches = trimmedLine.match(/CM_\sBO_\s(\d+)\s"(.+)";/);

        if (matches) {
          const [, canIdDec, comment] = matches;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.values) {
            currentMessage.values = [
              ...tableParams["dbc-can-messages-body"].defaultValues
            ];
          }

          currentMessage.values[MESSAGE_COMMENT] = comment;

          line.values = { canIdDec, comment };
          currentMessage.commentLine = line;

          currentMessageCanId = canIdDec;
          isMessageCommentLine = true;
        }
      } else if (trimmedLine.startsWith("CM_ SG_")) {
        // Line is comment for a signal

        /**
         * Comment format:
         *
         * `CM_ SG_ {CAN_ID} {NAME} "{COMMENT}";`
         *
         * - `{CAN_ID}`: decimal value
         * - `{NAME}`: 1-32 characters with letters, digits and underscores
         * - `{COMMENT}`: string
         *
         * Example:
         *
         * `CM_ SG_ 2364540158 EngineSpeed "Actual engine speed which is calculated over a minimum crankshaft angle of 720 degrees divided by the number of cylinders.…";`
         */
        matches = trimmedLine.match(/CM_\sSG_\s(\d+)\s(\w+)\s"(.+)";/);

        if (matches) {
          const [, canIdDec, name, comment] = matches;
          const id = name;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.signals) {
            currentMessage.signals = {};
          }

          if (!currentMessage.signalsOrder) {
            currentMessage.signalsOrder = [];
          }

          if (!currentMessage.selectedSignalId) {
            currentMessage.selectedSignalId = id;
          }

          let currentSignal = currentMessage.signals[id];

          if (!currentSignal) {
            currentSignal = {};
            currentMessage.signals[id] = currentSignal;
            currentMessage.signalsOrder.push(id);
          }

          if (!currentSignal.values) {
            currentSignal.values = [
              ...tableParams["dbc-can-signals-body"].defaultValues
            ];
          }

          currentSignal.values[SIGNAL_COMMENT] = comment;

          line.values = { canIdDec, name, comment };
          currentSignal.commentLine = line;

          wasPrevLineSignalComment = true;
          isSignalCommentLine = true;
        }
      } else if (trimmedLine.startsWith("BA_") && trimmedLine.includes("BO_")) {
        // Line is metadata for a message

        /**
         * Metadata format:
         *
         * `BA_ "{KEY}" BO_ {CAN_ID} {VALUE};`
         *
         * - `{KEY}`: string
         * - `{CAN_ID}`: decimal value
         * - `{VALUE}`: string
         *
         * Example:
         *
         * `BA_ "VFrameFormat" BO_ 2364540158 3;`
         */
        matches = trimmedLine.match(/BA_\s"(.+)"\sBO_\s(\d+)\s(.+);/);

        if (matches) {
          const [, key, canIdDec, value] = matches;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.metadata) {
            currentMessage.metadata = {};
          }

          if (!currentMessage.metadataLines) {
            currentMessage.metadataLines = [];
          }

          currentMessage.metadata[key] = value;

          line.values = { key, canIdDec, value };
          currentMessage.metadataLines.push(line);
          isTypeLine = true;
        }
      } else if (trimmedLine.startsWith("BA_") && trimmedLine.includes("SG_")) {
        // Line is metadata for a signal

        /**
         * Metadata format:
         *
         * `BA_ "{KEY}" SG_ {CAN_ID} {NAME} {VALUE};`
         *
         * - `{KEY}`: string
         * - `{CAN_ID}`: decimal value
         * - `{NAME}`: 1-32 characters with letters, digits and underscores
         * - `{VALUE}`: string
         *
         * Example:
         *
         * `BA_ "SPN" SG_ 2364540158 EngineSpeed 190;`
         */
        matches = trimmedLine.match(/BA_\s"(.+)"\sSG_\s(\d+)\s(\w+)\s(.+);/);

        if (matches) {
          const [, key, canIdDec, name, value] = matches;
          const id = name;

          let currentMessage = messagesByCanId[canIdDec];

          if (!currentMessage) {
            currentMessage = {};
            messagesByCanId[canIdDec] = currentMessage;
          }

          if (!currentMessage.signals) {
            currentMessage.signals = {};
          }

          if (!currentMessage.signalsOrder) {
            currentMessage.signalsOrder = [];
          }

          if (!currentMessage.selectedSignalId) {
            currentMessage.selectedSignalId = id;
          }

          let currentSignal = currentMessage.signals[id];

          if (!currentSignal) {
            currentSignal = {};
            currentMessage.signals[id] = currentSignal;
            currentMessage.signalsOrder.push(id);
          }

          if (!currentSignal.metadata) {
            currentSignal.metadata = {};
          }

          if (!currentSignal.metadataLines) {
            currentSignal.metadataLines = [];
          }

          currentSignal.metadata[key] = value;

          line.values = { key, canIdDec, name, value };
          currentSignal.metadataLines.push(line);
        }
      } else if (trimmedLine.startsWith('BA_ "ProtocolType"')) {
        protocolLine = line;
      }

      if (wasPrevLineSignal && !isSignalLine) {
        let currentMessage = messagesByCanId[currentMessageCanId];

        const newSignalsLines = [];
        currentMessage.newSignalsLines = newSignalsLines;
        lines.push(newSignalsLines);

        wasPrevLineSignal = false;
        i += 1;
      }

      if (wasPrevLineSignalComment && !isSignalCommentLine) {
        let currentMessage = messagesByCanId[currentMessageCanId];

        const newSignalsCommentsLines = [];
        currentMessage.newCommentSignalsLine = newSignalsCommentsLines;
        lines.push(newSignalsCommentsLines);

        wasPrevLineSignalComment = false;
        i += 1;
      }

      lines.push(line);

      if (isMessageLine || isSignalLine) {
        lastMessageLine = i;
      } else if (isMessageCommentLine || isSignalCommentLine) {
        lastCommentLine = i;
      } else if (isSignalMultiplexLine) {
        lastMultiplexLine = i;
      } else if (isTypeLine) {
        lastTypeLine = i;
      }
      i += 1;
    }

    if (lastMessageLine === 0) {
      newMessagesLines.push({
        type: "blank",
        rawLine: ""
      });
      lastMessageLine = lines.length - 1;
    }

    if (lastCommentLine === 0) {
      newCommentsLines.push({
        type: "blank",
        rawLine: ""
      });
      lastCommentLine = lines.length - 1;
    }

    if (lastMessageLine > lastCommentLine) {
      lines.splice(lastMessageLine + 1, 0, newMessagesLines);
      lines.splice(lastCommentLine + 1, 0, newCommentsLines);
    } else {
      lines.splice(lastCommentLine + 1, 0, newCommentsLines);
      lines.splice(lastMessageLine + 1, 0, newMessagesLines);
    }

    if (lastMultiplexLine === 0) {
      newMultiplexLines.push({
        type: "blank",
        rawLine: ""
      });
      lastMultiplexLine = lines.length - 1;
    }

    if (lastTypeLine === 0) {
      newTypesLines.push({
        type: "blank",
        rawLine: ""
      });
      lastTypeLine = lines.length - 1;
    }
    lines.splice(lastMultiplexLine + 1, 0, newMultiplexLines);
    lines.splice(lastTypeLine + 3, 0, newTypesLines);

    const messagesToInsert = [];

    for (let message of Object.values(messagesByCanId)) {
      message = {
        cells: [],
        values: [...tableParams["dbc-can-messages-body"].defaultValues],
        signals: {},
        signalsOrder: [],
        selectedSignalId: "",
        line: {
          type: "message",
          isNew: true,
          rawLine: "",
          values: {}
        },
        commentLine: {
          type: "message-comment",
          isNew: true,
          rawLine: "",
          values: {}
        },
        typeLine: {
          type: "message-type",
          isNew: true,
          rawLine: "",
          values: {}
        },
        metadata: {},
        metadataLines: [],
        newSignalsLines: [],
        newSignalsCommentsLines: [],
        ...message
      };

      if (message.line.isNew) {
        newMessagesLines.push(message.line);
        newMessagesLines.push(message.newSignalsLines);
      }

      if (message.commentLine.isNew) {
        newCommentsLines.push(message.commentLine);
        newCommentsLines.push(message.newSignalsCommentsLines);
      }

      message.signals = Object.fromEntries(
        Object.entries(message.signals).map(([signalId, signal]) => {
          signal = {
            cells: [],
            values: [...tableParams["dbc-can-signals-body"].defaultValues],
            signalMask: [],
            activeBitIndices: [],
            lsb: 0,
            msb: 0,
            line: {
              type: "signal",
              isNew: true,
              rawLine: "",
              values: {}
            },
            commentLine: {
              type: "signal-comment",
              isNew: true,
              rawLine: "",
              values: {}
            },
            multiplexLine: {
              type: "multiplex",
              isNew: true,
              rawLine: "",
              values: {}
            },
            metadata: {},
            metadataLines: [],
            ...signal
          };

          if (signal.line.isNew) {
            message.newSignalsLines.push(signal.line);
          }

          if (signal.commentLine.isNew) {
            message.newSignalsCommentsLines.push(signal.commentLine);
          }

          if (signal.values[SIGNAL_MODE] !== "multiplexingGroup") {
            signal.multiplexLine.isDeleted = true;
          }

          if (signal.multiplexLine.isNew) {
            newMultiplexLines.push(signal.multiplexLine);
          }

          return [signalId, signal];
        })
      );

      const name = message.values[MESSAGE_NAME];
      const id = name;
      canMessages[id] = message;

      const line = message.line;
      const canIdDec = strToDec(line.values.canIdDec);

      if (canIdDec <= 0x7ff) {
        // ID is 11-bit long

        message.values[MESSAGE_TYPE] = "standard";
      } else {
        //  ID is 32-bit long

        message.values[MESSAGE_TYPE] =
          message.metadata.VFrameFormat === "3"
            ? "extendedJ1939PG"
            : "extended";
        message.values[MESSAGE_CAN_ID] = dbcIdToCanId(
          canIdDec,
          message.values[MESSAGE_TYPE],
          "dec"
        );
      }

      messagesToInsert.push(message.values);
    }
    
    // After parsing all lines, check if we need to add the VFrameFormat attribute
    if (!hasVFrameFormat && firstBaDefAfterBsIndex !== -1) {
      // Get the line object structure from the line we're inserting before to ensure consistency
      const targetLine = lines[firstBaDefAfterBsIndex];
      
      // Create the new line object with the exact same properties as the target line
      const vFrameFormatLine = {};
      
      // Copy all properties from the target line to ensure consistent structure
      Object.keys(targetLine).forEach(key => {
        if (key !== 'rawLine' && key !== 'type') {
          vFrameFormatLine[key] = targetLine[key];
        }
      });
      
      // Set the specific properties we need for our line
      vFrameFormatLine.type = "attribute-definition";
      vFrameFormatLine.rawLine = 'BA_DEF_ BO_  "VFrameFormat" ENUM  "StandardCAN","ExtendedCAN","reserved","J1939PG";';
      
      // Insert it directly before the first BA_DEF_ line that appears after the BS_: line
      lines.splice(firstBaDefAfterBsIndex, 0, vFrameFormatLine);
      
      // Adjust any indices that might be affected by this insertion
      if (lastMessageLine >= firstBaDefAfterBsIndex) lastMessageLine++;
      if (lastCommentLine >= firstBaDefAfterBsIndex) lastCommentLine++;
      if (lastMultiplexLine >= firstBaDefAfterBsIndex) lastMultiplexLine++;
      if (lastTypeLine >= firstBaDefAfterBsIndex) lastTypeLine++;
    }

    canMessagesBody.innerHTML = "";
    insertRows(canMessagesBody, null, messagesToInsert, false, true);
  };

  const openDiffModal = () => {
    //console.debug('Opening diff modal', lines);

    if (protocolLine) {
      const isProtocolEmpty = /BA_\s"ProtocolType"\s"";/.test(
        protocolLine.rawLine
      );
      if (isProtocolEmpty) {
        const anyJ1939Message = Object.values(canMessages).some(
          (message) => message.values[MESSAGE_TYPE] === "extendedJ1939PG"
        );
        protocolLine.rawLine = `BA_ "ProtocolType" "${anyJ1939Message ? "J1939" : ""
          }";`;
      }
    }

    if (newMessagesLines.length > 0 && newMessagesLines[0].type === "blank") {
      const nonDeletedMessagesLines = newMessagesLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedMessagesLines > 1) {
        newMessagesLines[0].isDeleted = false;
      } else {
        newMessagesLines[0].isDeleted = true;
      }
    }

    if (newCommentsLines.length > 0 && newCommentsLines[0].type === "blank") {
      const nonDeletedCommentsLines = newCommentsLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedCommentsLines > 1) {
        newCommentsLines[0].isDeleted = false;
      } else {
        newCommentsLines[0].isDeleted = true;
      }
    }

    if (newTypesLines.length > 0 && newTypesLines[0].type === "blank") {
      const nonDeletedTypeLines = newTypesLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedTypeLines > 1) {
        newTypesLines[0].isDeleted = false;
      } else {
        newTypesLines[0].isDeleted = true;
      }
    }

    if (newMultiplexLines.length > 0 && newMultiplexLines[0].type === "blank") {
      const nonDeletedMultiplexLines = newMultiplexLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedMultiplexLines > 1) {
        newMultiplexLines[0].isDeleted = false;
      } else {
        newMultiplexLines[0].isDeleted = true;
      }
    }

    const newFile = lines
      .flat(2)
      .filter(
        (line) => !line.isDeleted && (!!line.rawLine || line.type === "blank")
      )
      .map((line) => (line.type === "message" ? [line] : [line]))
      .flat()
      .map((line) => line.rawLine)
      .join(lineSeparator);

    if (file === newFile) {
      modalContent.innerHTML =
        '<div style="text-align: center;">No changes</div>';
    } else {
      modalContent.innerHTML = "";

      const diffPatch = Diff.createPatch("", file, newFile, "", "");
      const diff2htmlUi = new Diff2HtmlUI(modalContent, diffPatch, {
        drawFileList: false,
        fileContentToggle: false,
        matching: "lines",
        outputFormat: "side-by-side"
      });

      diff2htmlUi.draw();
    }

    const downloadButton = document.createElement("button");
    downloadButton.className = "dbc-header-button";
    downloadButton.type = "button";
    downloadButton.textContent = "download DBC";
    downloadButton.addEventListener("click", saveFile);

    modalActions.innerHTML = "";
    modalActions.appendChild(downloadButton);

    modalBackdrop.classList.remove("dbc-invisible");
  };

  const closeModal = () => {
    //console.debug('Closing modal');

    modalBackdrop.classList.add("dbc-invisible");
  };

  const saveFile = () => {
    //console.debug('Saving file');

    const a = document.createElement("a");
    a.style.display = "none";
    document.body.appendChild(a);

    //console.log("lines",lines.flat().filter((line) => !line.isDeleted && (!!line.rawLine || !line.isNew)).map((line) => line.rawLine).join(lineSeparator))

    if (protocolLine) {
      const isProtocolEmpty = /BA_\s"ProtocolType"\s"";/.test(
        protocolLine.rawLine
      );
      if (isProtocolEmpty) {
        const anyJ1939Message = Object.values(canMessages).some(
          (message) => message.values[MESSAGE_TYPE] === "extendedJ1939PG"
        );
        protocolLine.rawLine = `BA_ "ProtocolType" "${anyJ1939Message ? "J1939" : ""
          }";`;
      }
    }

    if (newMessagesLines.length > 0 && newMessagesLines[0].type === "blank") {
      const nonDeletedMessagesLines = newMessagesLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedMessagesLines > 1) {
        newMessagesLines[0].isDeleted = false;
      } else {
        newMessagesLines[0].isDeleted = true;
      }
    }

    if (newCommentsLines.length > 0 && newCommentsLines[0].type === "blank") {
      const nonDeletedCommentsLines = newCommentsLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedCommentsLines > 1) {
        newCommentsLines[0].isDeleted = false;
      } else {
        newCommentsLines[0].isDeleted = true;
      }
    }

    if (newTypesLines.length > 0 && newTypesLines[0].type === "blank") {
      const nonDeletedTypeLines = newTypesLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedTypeLines > 1) {
        newTypesLines[0].isDeleted = false;
      } else {
        newTypesLines[0].isDeleted = true;
      }
    }

    if (newMultiplexLines.length > 0 && newMultiplexLines[0].type === "blank") {
      const nonDeletedMultiplexLines = newMultiplexLines.filter(
        (line) => !line.isDeleted
      ).length;
      if (nonDeletedMultiplexLines > 1) {
        newMultiplexLines[0].isDeleted = false;
      } else {
        newMultiplexLines[0].isDeleted = true;
      }
    }

    const newFile = lines
      .flat(2)
      .filter(
        (line) => !line.isDeleted && (!!line.rawLine || line.type === "blank")
      )
      .map((line) =>
        line.type === "message"
          ? [{ type: "blank", rawLine: "" }, line]
          : [line]
      )
      .flat()
      .map((line) => line.rawLine)
      .join(lineSeparator);

    /*
    const newFile = lines
      .flat()
      .filter((line) => !line.isDeleted && (!!line.rawLine || !line.isNew))
      .map((line) => line.rawLine)
      .join(lineSeparator);
  	
    console.log("newFile",newFile)*/

    const blob = new Blob([newFile], { type: "octet/stream" });
    const url = URL.createObjectURL(blob);

    a.href = url;
    a.download = filename || "file.dbc";
    a.dispatchEvent(new MouseEvent("click"));
    URL.revokeObjectURL(url);
  };

  /**
   * @param {MouseEvent} event
   */
  const checkClick = (event) => {
    //console.debug('Checking click', event);

    /** @type {HTMLElement} */
    const target = event.target;

    if (target.classList.contains("dbc-add-button")) {
      /** @type {HTMLElement} */
      const header = target.parentElement.parentElement;

      switch (header.id) {
        case "dbc-can-messages-header": {
          insertRows(
            canMessagesBody,
            null,
            [[...tableParams["dbc-can-messages-body"].defaultValues]],
            true
          );
          updateTables(event);

          const row =
            canMessagesBody.parentElement.rows[
            canMessagesBody.parentElement.rows.length - 1
            ];
          if (!row.classList.contains("dbc-selected")) {
            const params = tableParams["dbc-can-messages-body"];
            //const params = tableParams[body.id];
            updateSelectedRow(params, row);
          }
          break;
        }

        case "dbc-can-signals-header": {
          insertRows(
            canSignalsBody,
            null,
            [[...tableParams["dbc-can-signals-body"].defaultValues]],
            true
          );
          updateTables(event);
          const row =
            canSignalsBody.parentElement.rows[
            canSignalsBody.parentElement.rows.length - 1
            ];
          if (!row.classList.contains("dbc-selected")) {
            const params = tableParams["dbc-can-signals-body"];
            updateSelectedRow(params, row);
          }
          break;
        }

        default:
          break;
      }
    } else if (target.classList.contains("dbc-remove-button")) {
      /** @type {HTMLElement} */
      const header = target.parentElement.parentElement;

      switch (header.id) {
        case "dbc-can-messages-header": {
          removeSelectedMessage();

          break;
        }

        case "dbc-can-signals-header": {
          removeSelectedSignal();

          break;
        }

        default:
          break;
      }
    } else if (target.classList.contains("dbc-radio")) {
      /** @type {HTMLTableRowElement} */
      const row = target.parentElement.parentElement;

      if (!row.classList.contains("dbc-selected")) {
        /** @type {HTMLElement} */
        const body = row.parentElement;

        const params = tableParams[body.id];

        updateSelectedRow(params, row);
      }
    } else if (
      target.classList.contains("dbc-select") &&
      !target.disabled &&
      target.name === "multiplexor"
    ) {
      /** @type {HTMLTableRowElement} */
      const row = target.parentElement.parentElement;

      const { selectedRowId } = tableParams["dbc-can-messages-body"];
      const message = canMessages[selectedRowId];
      const rowId = row.getAttribute("data-id");

      const optionsEntries = [
        ["None", "None"],
        ...message.signalsOrder
          .filter((signalId) => signalId !== rowId)
          .map((signalId) => [
            signalId,
            message.signals[signalId].values[SIGNAL_NAME]
          ])
      ];

      modalContent.innerHTML = "";
      modalActions.innerHTML = "";

      const optionsNodes = document.createElement("div");
      optionsNodes.className = "dbc-multiplexor-options";

      const selectedMultiplexor =
        message.signals[rowId].values[SIGNAL_MULTIPLEXOR];

      for (const [value, name] of optionsEntries) {
        const optionNode = document.createElement("div");
        optionNode.className = "dbc-multiplexor-option";
        if (value === selectedMultiplexor) {
          optionNode.classList.add("dbc-selected");
        }
        optionNode.setAttribute("data-value", value);
        optionNode.textContent = name;

        optionsNodes.appendChild(optionNode);
      }

      optionsNodes.addEventListener("click", (event) =>
        updateSelectedMultiplexor(event, row)
      );

      modalContent.appendChild(optionsNodes);

      modalBackdrop.classList.remove("dbc-invisible");
    } else if (target.classList.contains("dbc-collapse-button")) {
      /** @type {HTMLElement} */
      const collapsibleArea = target.parentElement.nextElementSibling;

      collapsibleArea.classList.toggle("dbc-hidden");
      target.classList.toggle("dbc-collapsed");
    }
  };

  const removeSelectedMessage = () => {
    //console.debug('Removing selected message');

    const params = tableParams["dbc-can-messages-body"];
    const { selectedRow: row, selectedRowId: rowId } = params;

    if (row && canMessagesBody.children.length > 1) {
      const message = canMessages[rowId];
      message.line.isDeleted = true;
      message.commentLine.isDeleted = true;
      message.typeLine.isDeleted = true;

      for (const metadataLine of message.metadataLines) {
        metadataLine.isDeleted = true;
      }

      for (const signal of Object.values(message.signals)) {
        signal.line.isDeleted = true;
        signal.commentLine.isDeleted = true;
        signal.multiplexLine.isDeleted = true;

        for (const metadataLine of signal.metadataLines) {
          metadataLine.isDeleted = true;
        }
      }

      row.remove();
      delete canMessages[rowId];

      updateSelectedRow(params, canMessagesBody.children[0]);
    }
  };

  const removeSelectedSignal = () => {
    //console.debug('Removig selected signal');

    const params = tableParams["dbc-can-signals-body"];
    const { selectedRow: row, selectedRowId: rowId } = params;

    if (row && canSignalsBody.children.length > 1) {
      const { selectedRowId: messageId } = tableParams[
        "dbc-can-messages-body"
      ];
      const message = canMessages[messageId];

      const signal = message.signals[rowId];
      signal.line.isDeleted = true;
      signal.commentLine.isDeleted = true;
      signal.multiplexLine.isDeleted = true;

      for (const metadataLine of signal.metadataLines) {
        metadataLine.isDeleted = true;
      }

      row.remove();
      delete message.signals[rowId];
      message.signalsOrder = message.signalsOrder.filter(
        (signalId) => signalId !== rowId
      );

      updateSelectedRow(params, canSignalsBody.children[0]);

      for (const otherSignal of Object.values(message.signals)) {
        if (otherSignal.values[SIGNAL_MULTIPLEXOR] === rowId) {
          /** @type {HTMLSelectElement} */
          const modeField = otherSignal.cells[SIGNAL_MODE].firstElementChild;

          modeField.value = "signal";

          modeField.dispatchEvent(new Event("change", { bubbles: true }));

          /** @type {HTMLSelectElement} */
          const multiplexorField =
            otherSignal.cells[SIGNAL_MULTIPLEXOR].firstElementChild;

          multiplexorField.options[0].value = "None";
          multiplexorField.options[0].textContent = "None";
          multiplexorField.value = "None";

          multiplexorField.dispatchEvent(
            new Event("change", { bubbles: true })
          );
        }
      }
    }
  };

  /**
   * @param {TableParams} params
   * @param {HTMLTableRowElement} rowToSelect
   */
  const updateSelectedRow = (params, rowToSelect) => {
    //console.debug('Updating selected row', params, rowToSelect);

    const { tableId, selectedRow } = params;

    if (selectedRow) {
      selectedRow.classList.remove("dbc-selected");
    }

    rowToSelect.classList.add("dbc-selected");

    const rowId = rowToSelect.getAttribute("data-id");
    params.selectedRow = rowToSelect;
    params.selectedRowId = rowId;

    switch (tableId) {
      case "can-messages": {
        for (const message of Object.values(canMessages)) {
          for (const signal of Object.values(message.signals)) {
            signal.row = null;
            signal.cells = [];
          }
        }

        const { values, signals, signalsOrder, selectedSignalId } = canMessages[
          rowId
        ];

        canSignalsBody.innerHTML = "";
        insertRows(
          canSignalsBody,
          null,
          signalsOrder.map((signalId) => [signalId, signals[signalId].values]),
          false
        );

        const signalParams = tableParams["dbc-can-signals-body"];
        updateSelectedRow(
          signalParams,
          canSignalsBody.querySelector(`[data-id="${selectedSignalId}"]`)
        );

        const nameNodes = document.querySelectorAll(
          ".dbc-selected-message-name"
        );

        for (const nameNode of nameNodes) {
          nameNode.title = values[MESSAGE_NAME];
          nameNode.textContent = values[MESSAGE_NAME];
        }

        break;
      }

      case "can-signals": {
        const { selectedRowId: messageId } = tableParams[
          "dbc-can-messages-body"
        ];
        canMessages[messageId].selectedSignalId = rowId;

        break;
      }

      default:
        break;
    }

    updateTables(null);
  };

  /**
   * @param {Event} event
   * @param {HTMLTableRowElement} row
   */
  const updateSelectedMultiplexor = (event, row) => {
    //console.debug('Updating selected multiplexor', event, row);

    /** @type {HTMLDivElement} */
    const optionNode = event.target;

    const optionValue = optionNode.getAttribute("data-value");
    const optionName = optionNode.textContent;

    /** @type {HTMLSelectElement} */
    const field = row.cells[SIGNAL_MULTIPLEXOR].firstElementChild;

    field.options[0].value = optionValue;
    field.options[0].textContent = optionName;
    field.value = optionValue;

    field.dispatchEvent(new Event("change", { bubbles: true }));

    closeModal();
  };

  /**
   * @param {Event} event
   */
  const changeNumberFormat = (event) => {
    //console.debug('Changing number format', event);

    prevNumberFormat = currNumberFormat;
    currNumberFormat = numberFormatSelect.value;

    for (const message of Object.values(canMessages)) {
      const oldValue = message.values[MESSAGE_CAN_ID];
      const newValue = formatNumber(
        oldValue,
        prevNumberFormat,
        currNumberFormat,
        8
      );
      message.values[MESSAGE_CAN_ID] = newValue;

      const currentCell = message.cells[MESSAGE_CAN_ID];

      if (currentCell) {
        currentCell.firstElementChild.value = newValue;
      }
    }
  };

  /**
   * @param {KeyboardEvent} event
   */
  const checkFieldKey = (event) => {
    //console.debug('Checking field key', event);

    /** @type {HTMLElement} */
    const body = event.currentTarget;

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = event.target;

    /** @type {HTMLElement} */
    const cell = field.parentElement;

    /** @type {HTMLTableRowElement} */
    const row = cell.parentElement;

    const key = event.key;
    const params = tableParams[body.id];
    const { fieldColumns, defaultValues, isEditable } = params;
    const currentIndex = Number.parseInt(cell.getAttribute("data-index"));

    switch (key) {
      case "ArrowDown":
      case "Down": {
        // If the down arrow key is pressed, move the focus to the field in the next row (if exists)

        focusOnNextRowField(event, row, currentIndex);

        break;
      }

      case "ArrowLeft":
      case "Left": {
        // If the left arrow key is pressed at the start, move the focus to the previous field (if exists)

        if (isFieldCursorAtStart(field)) {
          focusOnPreviousField(event, params, row, currentIndex);
        }

        break;
      }

      case "ArrowRight":
      case "Right": {
        // If the right arrow key is pressed at the end, move the focus to the next field (if exists)

        if (isFieldCursorAtEnd(field)) {
          focusOnNextField(event, params, row, currentIndex);
        }

        break;
      }

      case "ArrowUp":
      case "Up": {
        // If the up arrow key is pressed, move the focus to the field in the previous row (if exists)

        focusOnPreviousRowField(event, row, currentIndex);

        break;
      }

      case "Enter": {
        // If the enter key is pressed, create a new row below the current one

        if (isEditable) {
          event.preventDefault();
          event.stopImmediatePropagation();

          insertRows(body, row, [[...defaultValues]], false);
          focusOnNextRowField(event, row, currentIndex);
          updateTables(event);
        }

        break;
      }

      case "Tab": {
        // If the tab key is pressed in the last field and the next row doesn't exist, create one

        const lastFieldColumn = fieldColumns[fieldColumns.length - 1];

        if (isEditable && currentIndex === lastFieldColumn) {
          /** @type {HTMLTableRowElement | null} */
          const nextRow = row.nextElementSibling;

          if (!nextRow) {
            event.preventDefault();
            event.stopImmediatePropagation();

            insertRows(body, row, [[...defaultValues]], true);
            updateTables(event);
          }
        }

        break;
      }

      default:
        break;
    }
  };

  /**
   * If the field is a select, always returns `true`.
   *
   * @param {HTMLInputElement | HTMLSelectElement} field
   * @returns {boolean}
   */
  const isFieldCursorAtStart = (field) => {
    //console.debug('Checking if field cursor is at start', field);

    if ("selectionStart" in field && field.type === "text") {
      const selectionStart = field.selectionStart;
      const selectionEnd = field.selectionEnd;
      const selectionRange = selectionEnd - selectionStart;

      return selectionStart === 0 && selectionRange === 0;
    }

    return true;
  };

  /**
   * If the field is a select, always returns `true`.
   *
   * @param {HTMLInputElement | HTMLSelectElement} field
   * @returns {boolean}
   */
  const isFieldCursorAtEnd = (field) => {
    //console.debug('Checking if field cursor is at end', field);

    if ("selectionStart" in field && field.type === "text") {
      const selectionEnd = field.selectionEnd;
      const selectionStart = field.selectionStart;
      const selectionRange = selectionEnd - selectionStart;

      return selectionEnd === field.value.length && selectionRange === 0;
    }

    return true;
  };

  /**
   * If there's no previous field in the row, moves the focus to the field in the previous row instead (if exists).
   *
   * @param {KeyboardEvent} event
   * @param {TableParams} params
   * @param {HTMLTableRowElement} row
   * @param {number} currentIndex The index for the current cell.
   * @param {number} [selectionIndex] If the field is an input, where to position the cursor in the focused input. By default, the cursor maintains the same position. Providing `Infinity` moves the cursor to the end of the input.
   * @returns {HTMLElement | null} The cell or row containing the focused field, if one was found.
   */
  const focusOnPreviousField = (
    event,
    params,
    row,
    currentIndex,
    selectionIndex
  ) => {
    //console.debug('Focusing on previous field', event, params, row, currentIndex, selectionIndex);

    const { fieldColumns } = params;
    const fieldColumnIndex = fieldColumns.indexOf(currentIndex) - 1;

    if (fieldColumnIndex < 0) {
      const lastFieldColumn = fieldColumns[fieldColumns.length - 1];

      return focusOnPreviousRowField(
        event,
        row,
        lastFieldColumn,
        selectionIndex
      );
    }

    event.preventDefault();
    event.stopImmediatePropagation();

    const fieldColumn = fieldColumns[fieldColumnIndex];

    /** @type {HTMLTableCellElement} */
    const previousCell = row.cells[fieldColumn];

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = previousCell.firstElementChild;

    focusOnField(field, selectionIndex);

    return previousCell;
  };

  /**
   * If there's no next field in the row, moves the focus to the field in the next row instead (if exists).
   *
   * @param {KeyboardEvent} event
   * @param {TableParams} params
   * @param {HTMLTableRowElement} row
   * @param {number} currentIndex The index for the current cell.
   * @param {number} [selectionIndex] If the field is an input, where to position the cursor in the focused input. By default, the cursor maintains the same position. Providing `Infinity` moves the cursor to the end of the input.
   * @returns {HTMLElement | null} The cell or row containing the focused field, if one was found.
   */
  const focusOnNextField = (
    event,
    params,
    row,
    currentIndex,
    selectionIndex
  ) => {
    //console.debug('Focusing on next field', event, params, row, currentIndex, selectionIndex);

    const { fieldColumns } = params;
    const fieldColumnIndex = fieldColumns.indexOf(currentIndex) + 1;

    if (fieldColumnIndex >= fieldColumns.length) {
      const firstFieldColumn = fieldColumns[0];

      return focusOnNextRowField(event, row, firstFieldColumn, selectionIndex);
    }

    event.preventDefault();
    event.stopImmediatePropagation();

    const fieldColumn = fieldColumns[fieldColumnIndex];

    /** @type {HTMLTableCellElement} */
    const nextCell = row.cells[fieldColumn];

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = nextCell.firstElementChild;

    focusOnField(field, selectionIndex);

    return nextCell;
  };

  /**
   * @param {KeyboardEvent} event
   * @param {HTMLTableRowElement} row
   * @param {number} currentIndex The index for the current cell.
   * @param {number} [selectionIndex] If the field is an input, where to position the cursor in the focused input. By default, the cursor maintains the same position. Providing `Infinity` moves the cursor to the end of the input.
   * @returns {HTMLElement | null} The row containing the focused field, if one was found.
   */
  const focusOnPreviousRowField = (
    event,
    row,
    currentIndex,
    selectionIndex
  ) => {
    //console.debug('Focusing on previous row field', event, row, currentIndex, selectionIndex);

    /** @type {HTMLTableRowElement | null} */
    const previousRow = row.previousElementSibling;

    if (!previousRow) {
      return null;
    }

    event.preventDefault();
    event.stopImmediatePropagation();

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = previousRow.cells[currentIndex].firstElementChild;

    focusOnField(field, selectionIndex);

    return previousRow;
  };

  /**
   * @param {KeyboardEvent} event
   * @param {HTMLTableRowElement} row
   * @param {number} currentIndex The index for the current cell.
   * @param {number} [selectionIndex] If the field is an input, where to position the cursor in the focused input. By default, the cursor maintains the same position. Providing `Infinity` moves the cursor to the end of the input.
   * @returns {HTMLElement | null} The row containing the focused field, if one was found.
   */
  const focusOnNextRowField = (event, row, currentIndex, selectionIndex) => {
    //console.debug('Focusing on next row field', event, row, currentIndex, selectionIndex);

    /** @type {HTMLTableRowElement | null} */
    const nextRow = row.nextElementSibling;

    if (!nextRow) {
      return null;
    }

    event.preventDefault();
    event.stopImmediatePropagation();

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = nextRow.cells[currentIndex].firstElementChild;

    focusOnField(field, selectionIndex);

    return nextRow;
  };

  /**
   * @param {HTMLInputElement | HTMLSelectElement} field
   * @param {number} [selectionIndex] If the field is an input, where to position the cursor in the focused input. By default, the cursor maintains the same position. Providing `Infinity` moves the cursor to the end of the input.
   */
  const focusOnField = (field, selectionIndex) => {
    //console.debug('Focusing on field', field, selectionIndex);

    if (
      "selectionStart" in field &&
      field.type === "text" &&
      typeof selectionIndex !== "undefined"
    ) {
      if (selectionIndex === Infinity) {
        selectionIndex = field.value.length;
      }

      field.selectionStart = selectionIndex;
      field.selectionEnd = selectionIndex;
    }

    field.focus();
  };

  /**
   * @param {HTMLElement} body
   * @param {HTMLTableRowElement | null} refRow The reference row to insert the rows after, if any.
   * @param {unknown[][]} values The values to insert. Determines how many rows will be added. Each sub-array can also be a tuple in the `[string, unknown[]]` format, where the first element is the ID for the row and the second element is the actual row values.
   * @param {boolean} doFocus Whether or not to focus on the first inserted field, if any.
   * @param {boolean} doSelectFirstRow
   */
  const insertRows = (
    body,
    refRow,
    values,
    doFocus,
    doSelectFirstRow = false
  ) => {
    //console.debug('Inserting rows', body, refRow, values, doFocus);

    const fragment = document.createDocumentFragment();
    const params = tableParams[body.id];
    const { tableId, columns } = params;

    /** @type {[string, unknown[]][]} */
    const valuesEntries =
      values[0].length === 2 && Array.isArray(values[0][1])
        ? values
        : values.map((rowValues) => ["", rowValues]);

    /** @type {HTMLInputElement | HTMLSelectElement | null} */
    let fieldToFocus = null;

    let rowIndex = body.children.length;

    for (let [rowId, rowValues] of valuesEntries) {
      const row = document.createElement("tr");

      /** @type {HTMLTableCellElement[]} */
      let cells = [];

      for (const [cellIndex, value] of rowValues.entries()) {
        const cell = document.createElement("td");
        const column = columns[cellIndex];
        const valueStr = value.toString();

        switch (column.type) {
          case "input": {
            const input = document.createElement("input");
            input.classList.add("dbc-input");
            input.name = column.name;

            if (column.attributes) {
              for (const [attributeName, attributeValue] of Object.entries(
                column.attributes
              )) {
                input.setAttribute(attributeName, attributeValue);
              }
            }

            if (valueStr) {
              input.value = valueStr;
            }

            if (!fieldToFocus) {
              fieldToFocus = input;
            }

            cell.classList.add("dbc-input-container");
            cell.appendChild(input);

            if (column.isIdColumn) {
              let id = valueStr;

              switch (tableId) {
                case "can-messages": {
                  // Save the message with a unique ID

                  if (!id) {
                    id = rowId;
                  }

                  while (!id) {
                    id = generateRandomId();

                    if (canMessages[id]) {
                      id = "";
                    }
                  }

                  if (!canMessages[id]) {
                    const signalId = generateRandomId();
                    const signal = {
                      cells: [],
                      values: [
                        ...tableParams["dbc-can-signals-body"].defaultValues
                      ],
                      signalMask: [],
                      activeBitIndices: [],
                      lsb: 0,
                      msb: 0,
                      line: {
                        type: "signal",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      commentLine: {
                        type: "signal-comment",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      multiplexLine: {
                        type: "multiplex",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      metadata: {},
                      metadataLines: []
                    };
                    const message = {
                      cells: [],
                      values: rowValues,
                      signals: {
                        [signalId]: signal
                      },
                      signalsOrder: [signalId],
                      selectedSignalId: signalId,
                      line: {
                        type: "message",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      commentLine: {
                        type: "message-comment",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      typeLine: {
                        type: "message-type",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      metadata: {},
                      metadataLines: [],
                      newSignalsLines: [],
                      newSignalsCommentsLines: []
                    };
                    canMessages[id] = message;

                    message.line.rawLine = generateMessageLine(message);
                    message.commentLine.rawLine = generateMessageCommentLine(
                      message
                    );
                    message.typeLine.rawLine = generateMessageTypeLine(message);

                    signal.line.rawLine = generateSignalLine(message, signal);
                    signal.commentLine.rawLine = generateSignalCommentLine(
                      message,
                      signal
                    );
                    signal.multiplexLine.rawLine = generateSignalMultiplexLine(
                      message,
                      signal
                    );

                    if (signal.values[SIGNAL_MODE] !== "multiplexingGroup") {
                      signal.multiplexLine.isDeleted = true;
                    }

                    newMessagesLines.push(message.line);
                    newMessagesLines.push(message.newSignalsLines);
                    newCommentsLines.push(message.commentLine);
                    newTypesLines.push(message.typeLine);
                    // newMultiplexLines.push(message.typeLine);
                    newCommentsLines.push(message.newSignalsCommentsLines);
                    message.newSignalsLines.push(signal.line);
                    message.newSignalsCommentsLines.push(signal.commentLine);
                    newMultiplexLines.push(signal.multiplexLine);
                  }

                  break;
                }

                case "can-signals": {
                  // Save the signal with a unique ID

                  const { selectedRowId: messageId } = tableParams[
                    "dbc-can-messages-body"
                  ];
                  const currentMessage = canMessages[messageId];

                  if (!id) {
                    id = rowId;
                  }

                  while (!id) {
                    id = generateRandomId();

                    if (currentMessage.signals[id]) {
                      id = "";
                    }
                  }

                  if (!currentMessage.signals[id]) {
                    const signal = {
                      cells: [],
                      values: rowValues,
                      signalMask: [],
                      activeBitIndices: [],
                      lsb: 0,
                      msb: 0,
                      line: {
                        type: "signal",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      commentLine: {
                        type: "signal-comment",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      multiplexLine: {
                        type: "multiplex",
                        isNew: true,
                        rawLine: "",
                        values: {}
                      },
                      metadata: {},
                      metadataLines: []
                    };
                    currentMessage.signals[id] = signal;

                    signal.line.rawLine = generateSignalLine(
                      currentMessage,
                      signal
                    );
                    signal.commentLine.rawLine = generateSignalCommentLine(
                      currentMessage,
                      signal
                    );
                    signal.multiplexLine.rawLine = generateSignalMultiplexLine(
                      currentMessage,
                      signal
                    );

                    if (signal.values[SIGNAL_MODE] !== "multiplexingGroup") {
                      signal.multiplexLine.isDeleted = true;
                    }

                    currentMessage.newSignalsLines.push(signal.line);
                    currentMessage.newSignalsCommentsLines.push(
                      signal.commentLine
                    );
                    newMultiplexLines.push(signal.multiplexLine);

                    if (refRow) {
                      const refRowId = refRow.getAttribute("data-id");
                      const refRowIndex = currentMessage.signalsOrder.indexOf(
                        refRowId
                      );
                      currentMessage.signalsOrder.splice(refRowIndex, 0, id);
                    } else {
                      currentMessage.signalsOrder.push(id);
                    }
                  }

                  break;
                }

                default:
                  break;
              }

              row.setAttribute("data-id", id);
              rowId = id;
            }

            break;
          }

          case "radio": {
            const radio = document.createElement("div");
            radio.classList.add("dbc-radio");

            cell.classList.add("dbc-radio-container");
            cell.appendChild(radio);

            break;
          }

          case "select": {
            const select = document.createElement("select");
            select.classList.add("dbc-select");
            select.name = column.name;

            let options = column.options;

            if (column.name === "multiplexor") {
              select.addEventListener("mousedown", (e) => e.preventDefault());

              if (valueStr) {
                options = {
                  [valueStr]: valueStr
                };
              }
            }

            for (const [optionValue, optionName] of Object.entries(options)) {
              const option = document.createElement("option");
              option.value = optionValue;
              option.textContent = optionName;
              select.appendChild(option);
            }

            if (valueStr) {
              select.value = valueStr;
            } else {
              select.value = column.defaultOption;
            }

            if (!fieldToFocus) {
              fieldToFocus = select;
            }

            cell.classList.add("dbc-select-container");
            cell.appendChild(select);

            break;
          }

          default: {
            cell.title = valueStr;
            cell.textContent = valueStr;

            break;
          }
        }

        cell.setAttribute("data-index", cellIndex);
        row.appendChild(cell);
        cells.push(cell);
      }

      if (rowId) {
        switch (tableId) {
          case "can-messages": {
            const currentMessage = canMessages[rowId];
            currentMessage.row = row;
            currentMessage.cells = cells;

            break;
          }

          case "can-signals": {
            const { selectedRowId: messageId } = tableParams[
              "dbc-can-messages-body"
            ];
            const currentMessage = canMessages[messageId];
            const currentSignal = currentMessage.signals[rowId];
            currentSignal.row = row;
            currentSignal.cells = cells;

            break;
          }

          default:
            break;
        }
      }

      if (tableId === "can-signals") {
        /** @type {HTMLSelectElement} */
        const modeField = row.cells[SIGNAL_MODE].firstElementChild;

        /** @type {HTMLSelectElement} */
        const multiplexorField =
          row.cells[SIGNAL_MULTIPLEXOR].firstElementChild;

        /** @type {HTMLInputElement} */
        const multiplexValueField =
          row.cells[SIGNAL_MULTIPLEX_VALUE].firstElementChild;

        if (modeField.value === "multiplexingGroup") {
          row.setAttribute("data-multiplexing-group", "");
          multiplexorField.disabled = false;
          multiplexValueField.disabled = multiplexorField.value === "None";
        } else {
          row.removeAttribute("data-multiplexing-group");
          multiplexorField.disabled = true;
          multiplexValueField.disabled = true;
        }
      }

      row.setAttribute("data-index", rowIndex);
      fragment.appendChild(row);
      rowIndex += 1;
    }

    if (refRow) {
      body.insertBefore(fragment, refRow.nextElementSibling);
    } else {
      body.appendChild(fragment);
    }

    if (doSelectFirstRow) {
      updateSelectedRow(params, body.children[0]);
    }

    const { selectedRow: messageRow, selectedRowId: messageId } = tableParams[
      "dbc-can-messages-body"
    ];
    const message = canMessages[messageId];

    for (const row of body.children) {
      const rowId = row.getAttribute("data-id");
      const signal = message ? message.signals[rowId] : {};

      for (const cell of row.cells) {
        const cellIndex = Number.parseInt(cell.getAttribute("data-index"));
        const column = columns[cellIndex];
        const field = cell.firstElementChild;

        validateField(column, field, message, signal, tableId, params);
      }

      checkRowErrorAttribute(tableId, messageRow, row);
    }

    const isMultiplexingGroupSelected = !!body.querySelector(
      "[data-multiplexing-group]"
    );

    if (isMultiplexingGroupSelected) {
      body.parentElement.setAttribute("data-multiplexing-group", "");
    } else {
      body.parentElement.removeAttribute("data-multiplexing-group");
    }

    if (doFocus && fieldToFocus) {
      focusOnField(fieldToFocus);
    }
  };

  /**
   * @returns {string}
   */
  const generateRandomId = () => {
    //console.debug('Generating random ID');

    return Math.trunc(Math.random() * 1000000000).toString();
  };

  /**
   * @param {Event | null} event
   */
  const updateTables = (event) => {
    //console.debug('Updating tables', event);
    updateBitIndicesTable(event);
    updateCanFrameTable(event);

    updateSignalMaskTable(event);
  };

  /**
   * @param {Event} event
   */
  const updateRow = (event) => {
    /** @type {HTMLElement} */
    const body = event.currentTarget;

    /** @type {HTMLInputElement | HTMLSelectElement} */
    const field = event.target;

    /** @type {HTMLElement} */
    const cell = field.parentElement;

    /** @type {HTMLTableRowElement} */
    const row = cell.parentElement;

    const params = tableParams[body.id];
    const { tableId, columns, updateRowValues } = params;
    const rowIndex = Number.parseInt(row.getAttribute("data-index"));
    const cellIndex = Number.parseInt(cell.getAttribute("data-index"));
    const column = columns[cellIndex];
    let rowId = row.getAttribute("data-id");

    const { selectedRow: messageRow, selectedRowId: messageId } = tableParams[
      "dbc-can-messages-body"
    ];
    const message = canMessages[messageId];
    const signal = message ? message.signals[rowId] : {};

    const oldRowId = rowId;

    const isFieldValid = validateField(column, field, message, signal, params);

    if (column.type === "input" && isFieldValid && column.isIdColumn) {
      const id = field.value;

      if (id !== rowId) {
        switch (tableId) {
          case "can-messages": {
            canMessages[id] = canMessages[rowId];
            delete canMessages[rowId];
            row.setAttribute("data-id", id);

            /** @type {HTMLInputElement} */
            const nameField = row.querySelector('[name="name"]');

            const nameNodes = document.querySelectorAll(
              ".dbc-selected-message-name"
            );

            for (const nameNode of nameNodes) {
              nameNode.title = nameField.value;
              nameNode.textContent = nameField.value;
            }

            if (params.selectedRowId === rowId) {
              params.selectedRowId = id;
            }

            break;
          }

          case "can-signals": {
            const { selectedRowId: messageId } = tableParams[
              "dbc-can-messages-body"
            ];
            const message = canMessages[messageId];

            message.signals[id] = message.signals[rowId];
            delete message.signals[rowId];
            row.setAttribute("data-id", id);

            message.signalsOrder = message.signalsOrder.map((signalId) => {
              if (signalId === rowId) {
                return id;
              }

              return signalId;
            });

            if (message.selectedSignalId === rowId) {
              message.selectedSignalId = id;
            }

            if (params.selectedRowId === rowId) {
              params.selectedRowId = id;
            }

            break;
          }

          default:
            break;
        }

        rowId = id;
      }
    }

    checkRowErrorAttribute(tableId, messageRow, row);
    if (column.name === "mode") {
      /** @type {HTMLSelectElement} */
      const multiplexorField = row.cells[SIGNAL_MULTIPLEXOR].firstElementChild;

      /** @type {HTMLInputElement} */
      const multiplexValueField =
        row.cells[SIGNAL_MULTIPLEX_VALUE].firstElementChild;

      if (field.value === "multiplexingGroup") {
        row.setAttribute("data-multiplexing-group", "");
        multiplexorField.disabled = false;
        multiplexValueField.disabled = multiplexorField.value === "None";
      } else {
        row.removeAttribute("data-multiplexing-group");
        multiplexorField.disabled = true;
        multiplexValueField.disabled = true;
      }
    } else if (column.name === "multiplexor") {
      /** @type {HTMLInputElement} */
      const multiplexValueField =
        row.cells[SIGNAL_MULTIPLEX_VALUE].firstElementChild;

      multiplexValueField.disabled = field.value === "None";
    }

    const isMultiplexingGroupSelected = !!body.querySelector(
      "[data-multiplexing-group]"
    );

    if (isMultiplexingGroupSelected) {
      body.parentElement.setAttribute("data-multiplexing-group", "");
    } else {
      body.parentElement.removeAttribute("data-multiplexing-group");
    }

    if (updateRowValues) {
      updateRowValues(
        event,
        params,
        row,
        rowId,
        rowIndex,
        cell,
        cellIndex,
        field
      );
    }

    updateTables(event);

    if (
      column.type === "input" &&
      column.isIdColumn &&
      tableId === "can-signals"
    ) {
      const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];
      const message = canMessages[messageId];

      for (const signal of Object.values(message.signals)) {
        if (signal.values[SIGNAL_MULTIPLEXOR] === oldRowId) {
          const id = field.value;

          /** @type {HTMLSelectElement} */
          const multiplexorField =
            signal.cells[SIGNAL_MULTIPLEXOR].firstElementChild;

          multiplexorField.options[0].value = id;
          multiplexorField.options[0].textContent = id;
          multiplexorField.value = id;

          multiplexorField.dispatchEvent(
            new Event("change", { bubbles: true })
          );
        }
      }
    }

    if (column.name === "mode") {
      const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];
      const message = canMessages[messageId];

      for (const signal of Object.values(message.signals)) {
        if (signal.values[SIGNAL_MULTIPLEXOR] === rowId) {
          signal.line.rawLine = generateSignalLine(message, signal);
        }
      }
    } else if (column.name === "order") {
      const { selectedRowId: signalId } = tableParams["dbc-can-signals-body"];
      const signal = canMessages[messageId].signals[signalId];
      const isLittleEndian =
        ENDIAN_MAP[signal.values[SIGNAL_ORDER]] === "little";
      for (let i = 0, n = bitIndices.length; i < n; i++) {
        bitIndicesBody.children[i].cells[0].textContent = isLittleEndian
          ? n - i - 1
          : i;
      }
    }
  };

  const validateField = (column, field, message, signal, params) => {
    let isFieldValid = true;

    if (column.type === "input") {
      const { validate } = column;

      if (validate) {
        const err = validate(field, message, signal);

        if (err) {
          field.setCustomValidity(err);
          reportValidityIfFocused(field);

          isFieldValid = false;
        } else {
          field.setCustomValidity("");

          if (!field.checkValidity()) {
            reportValidityIfFocused(field);

            isFieldValid = false;
          }
        }
      } else if (!field.checkValidity()) {
        reportValidityIfFocused(field);

        isFieldValid = false;
      }
    }

    if (column.dependencies) {
      const { tableId, columns } = params;
      let messageOrSignal;

      switch (tableId) {
        case "can-messages":
          messageOrSignal = message;

          break;

        case "can-signals":
          messageOrSignal = signal;

          break;

        default:
          break;
      }

      if (messageOrSignal) {
        for (const dependency of column.dependencies) {
          validateField(
            columns[dependency],
            messageOrSignal.cells[dependency].firstElementChild,
            message,
            signal,
            params
          );
        }
      }
    }

    return isFieldValid;
  };

  /**
   * @param {HTMLInputElement} input
   */
  const reportValidityIfFocused = (input) => {
    if (input === document.activeElement) {
      input.reportValidity();
    }
  };

  const checkRowErrorAttribute = (tableId, messageRow, row) => {
    switch (tableId) {
      case "can-messages": {
        if (
          row.querySelector(".dbc-input:invalid") ||
          canSignalsBody.querySelector(".dbc-input:invalid")
        ) {
          row.setAttribute("data-has-errors", "");
        } else {
          row.removeAttribute("data-has-errors");
        }

        break;
      }

      case "can-signals": {
        if (row.querySelector(".dbc-input:invalid")) {
          row.setAttribute("data-has-errors", "");
          messageRow.setAttribute("data-has-errors", "");
        } else {
          row.removeAttribute("data-has-errors");
          messageRow.removeAttribute("data-has-errors");
        }
      }

      default:
        break;
    }
  };

  /**
   * @param {Event | null} event
   * @param {TableParams} params
   * @param {HTMLTableRowElement} row
   * @param {string} rowId
   * @param {number} rowIndex
   * @param {HTMLTableCellElement} cell
   * @param {number} cellIndex
   * @param {HTMLInputElement | HTMLSelectElement} field
   */
  const updateCanMessagesRowValues = (
    event,
    params,
    row,
    rowId,
    rowIndex,
    cell,
    cellIndex,
    field
  ) => {
    // console.debug(
    // 	'Updating CAN messages row values',
    // 	event,
    // 	params,
    // 	row,
    // 	rowId,
    // 	rowIndex,
    // 	cell,
    // 	cellIndex,
    // 	field,
    // 	canMessages
    // );

    const { columns } = params;
    const column = columns[cellIndex];
    const message = canMessages[rowId];
    const { values, line, commentLine, typeLine } = message;

    // Store the old values before any updates
    const oldCanId = values[MESSAGE_CAN_ID];
    const oldType = values[MESSAGE_TYPE];
    const oldDbcId = canIdToDbcId(
      oldCanId,
      oldType,
      currNumberFormat,
      "dec"
    );

    // Determine if we're updating the CAN ID or message type
    const isCanIdUpdate = cellIndex === MESSAGE_CAN_ID;
    const isTypeUpdate = cellIndex === MESSAGE_TYPE;
    const needsMetaUpdate = isCanIdUpdate || isTypeUpdate;

    if (column.transform) {
      values[cellIndex] = column.transform(field.value, values);
    } else {
      values[cellIndex] = field.value;
    }

    // Update the main message line, type line, and comment line
    line.rawLine = generateMessageLine(message);
    typeLine.rawLine = generateMessageTypeLine(message);
    commentLine.rawLine = generateMessageCommentLine(message);

    // If this is a CAN ID update or type update (Standard <-> Extended), update all meta sections
    if (needsMetaUpdate) {
      const newCanId = values[MESSAGE_CAN_ID];
      const newType = values[MESSAGE_TYPE];
      const newDbcId = canIdToDbcId(
        newCanId, 
        newType,
        currNumberFormat,
        "dec"
      );

      // Only proceed with meta updates if the DBC ID actually changed
      if (oldDbcId !== newDbcId) {
        // Update signal comment lines
        if (message.signals) {
          Object.values(message.signals).forEach(signal => {
            if (signal.commentLine && signal.commentLine.rawLine) {
              // Update the signal comment line with the new CAN ID
              signal.commentLine.rawLine = generateSignalCommentLine(message, signal);
            }
            
            // Update signal metadata lines (BA_ entries)
            if (signal.metadataLines && signal.metadataLines.length > 0) {
              signal.metadataLines.forEach(metaLine => {
                if (metaLine.rawLine && metaLine.rawLine.includes(` SG_ ${oldDbcId} `)) {
                  // Replace the old CAN ID with the new one in BA_ entries for signals
                  metaLine.rawLine = metaLine.rawLine.replace(
                    ` SG_ ${oldDbcId} `,
                    ` SG_ ${newDbcId} `
                  );
                }
              });
            }
          });
        }

        // Update message metadata lines (BA_ entries)
        if (message.metadataLines && message.metadataLines.length > 0) {
          message.metadataLines.forEach(metaLine => {
            if (metaLine.rawLine && metaLine.rawLine.includes(` BO_ ${oldDbcId} `)) {
              // Replace the old CAN ID with the new one in BA_ entries for messages
              metaLine.rawLine = metaLine.rawLine.replace(
                ` BO_ ${oldDbcId} `,
                ` BO_ ${newDbcId} `
              );
            }
          });
        }

        // Update value tables (VAL_ entries)
        lines.flat(2).forEach(line => {
          if (line.rawLine && line.rawLine.startsWith(`VAL_ ${oldDbcId} `)) {
            // Replace the old CAN ID with the new one in VAL_ entries
            line.rawLine = line.rawLine.replace(
              `VAL_ ${oldDbcId} `,
              `VAL_ ${newDbcId} `
            );
          }
        });
      }
    }
  };

  /**
   * @param {Event | null} event
   * @param {TableParams} params
   * @param {HTMLTableRowElement} row
   * @param {string} rowId
   * @param {number} rowIndex
   * @param {HTMLTableCellElement} cell
   * @param {number} cellIndex
   * @param {HTMLInputElement | HTMLSelectElement} field
   */
  const updateCanSignalsRowValues = (
    event,
    params,
    row,
    rowId,
    rowIndex,
    cell,
    cellIndex,
    field
  ) => {
    // console.debug(
    // 	'Updating CAN signals row values',
    // 	event,
    // 	params,
    // 	row,
    // 	rowId,
    // 	rowIndex,
    // 	cell,
    // 	cellIndex,
    // 	field,
    // 	canMessages
    // );

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const { columns } = params;
    const column = columns[cellIndex];
    const message = canMessages[messageId];
    const signal = message.signals[rowId];
    const { values, line, commentLine, multiplexLine } = signal;

    if (column.transform) {
      values[cellIndex] = column.transform(field.value, values);
    } else {
      values[cellIndex] = field.value;
    }

    line.rawLine = generateSignalLine(message, signal);
    commentLine.rawLine = generateSignalCommentLine(message, signal);
    multiplexLine.rawLine = generateSignalMultiplexLine(message, signal);
    multiplexLine.isDeleted =
      signal.values[SIGNAL_MODE] !== "multiplexingGroup";
  };

  /**
   * @param {Event | null} event
   */
  const updateBitIndicesTable = (event) => {
    //console.debug('Updating bit indices table', event, bitIndices);
    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];
    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const currentDlc = bitIndices.length;
    const dlc = canMessages[messageId].values[MESSAGE_DLC];

    if (currentDlc === dlc) {
      //console.debug('No changes');
      // return;
    }

    /*
    const signal = canMessages[messageId].signals[signalId];
    const isLittleEndian = ENDIAN_MAP[signal.values[SIGNAL_ORDER]] === 'little';
    */
    // const { selectedRowId: signalId } = tableParams['dbc-can-signals-body'];
    const { selectedRowId: signaltempId } = tableParams["dbc-can-signals-body"];
    const signaltemp = canMessages[messageId].signals[signaltempId];
    const isLittleEndiantemp =
      ENDIAN_MAP[signaltemp.values[SIGNAL_ORDER]] === "little";

    bitIndicesBody.replaceChildren();
    while (bitIndices.length > 0) {
      bitIndices.pop();
    }
    while (bitIndices.length > 0) {
      bitIndices1.pop();
    }
    if (isLittleEndiantemp) {
      const newBitIndices = [];
      const newBitIndices1 = [];

      let diff = dlc - currentDlc;
      for (var i = 0; i < dlc; i++) {
        var values = [...new Array(8).keys()].map(
          (j) => (dlc - i) * 8 - (j + 1)
        );
        newBitIndices.push(["", ...values]);
      }

      insertRows(bitIndicesBody, null, newBitIndices, false);
      bitIndices.push(...newBitIndices);
    } else {
      const newBitIndices = [];
      const newBitIndices1 = [];

      let diff = dlc - currentDlc;

      for (var i = 0; i < dlc; i++) {
        var values = [...new Array(8).keys()].map((j) => (i + 1) * 8 - (j + 1));
        newBitIndices.push(["", ...values]);
      }
      for (var i = 0; i < dlc; i++) {
        var values1 = [...new Array(8).keys()].map(
          (j) => (i + 1) * 8 - (8 - j)
        );
        newBitIndices1.push(["", ...values1]);
      }

      insertRows(bitIndicesBody, null, newBitIndices, false);
      bitIndices.push(...newBitIndices);
      bitIndices1.push(...newBitIndices1);
    }

    const { selectedRowId: signalId } = tableParams["dbc-can-signals-body"];
    const signal = canMessages[messageId].signals[signalId];
    const isLittleEndian = ENDIAN_MAP[signal.values[SIGNAL_ORDER]] === "little";

    for (let i = 0, n = bitIndices.length; i < n; i++) {
      bitIndicesBody.children[i].cells[0].textContent = isLittleEndian
        ? n - i - 1
        : i;
    }
  };

  /**
   * @param {Event | null} event
   */
  const updateCanFrameTable = (event) => {
    //console.debug('Updating CAN frame table', event, canFrame);

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const currentDlc = canFrame.length;
    const dlc = canMessages[messageId].values[MESSAGE_DLC];

    if (currentDlc === dlc) {
      //console.debug('No changes');

      return;
    } else if (currentDlc < dlc) {
      // Insert missing rows

      const newCanFrame = [];

      let diff = dlc - currentDlc;
      let i = currentDlc;

      while (diff > 0) {
        let dataHex = "FF";

        if (isFirstCanFrameInsertion) {
          // Initialize with default values

          if (i === 3) {
            dataHex = "22";
          } else if (i === 4) {
            dataHex = "13";
          }
        }

        const values = [i, dataHex, "1".repeat(8), ...new Array(8).fill(1)];

        if (dataHex !== "FF") {
          updateCanFrameValues(values);
        }

        newCanFrame.push(values);

        i += 1;
        diff -= 1;
      }

      isFirstCanFrameInsertion = false;

      insertRows(canFrameBody, null, newCanFrame, false);
      canFrame.push(...newCanFrame);
    } else if (dlc < currentDlc) {
      // Remove extra rows

      let diff = currentDlc - dlc;

      while (diff > 0) {
        canFrameBody.lastElementChild.remove();
        canFrame.pop();

        diff -= 1;
      }
    }
  };

  /**
   * @param {Event | null} event
   * @param {TableParams} params
   * @param {HTMLTableRowElement} row
   * @param {string} rowId
   * @param {number} rowIndex
   * @param {HTMLTableCellElement} cell
   * @param {number} cellIndex
   * @param {HTMLInputElement | HTMLSelectElement} field
   */
  const updateCanFrameRowValues = (
    event,
    params,
    row,
    rowId,
    rowIndex,
    cell,
    cellIndex,
    field
  ) => {
    // console.debug(
    // 	'Updating CAN frame row values',
    // 	event,
    // 	params,
    // 	row,
    // 	rowId,
    // 	rowIndex,
    // 	cell,
    // 	cellIndex,
    // 	field,
    // 	canFrame
    // );

    const { columns } = params;
    const column = columns[cellIndex];
    const values = canFrame[rowIndex];

    if (column.transform) {
      values[cellIndex] = column.transform(field.value, values);
    } else {
      values[cellIndex] = field.value;
    }

    updateCanFrameValues(values);

    for (
      let i = cellIndex + 1, numCells = row.cells.length;
      i < numCells;
      i++
    ) {
      row.cells[i].title = values[i];
      row.cells[i].textContent = values[i];
    }
  };

  /**
   * @param {unknown[]} values
   */
  const updateCanFrameValues = (values) => {
    //console.debug('Updating CAN frame values', values);

    const dataHex = values[FRAME_DATA_HEX];
    const dataBin = hexToBin(dataHex, 8);

    values[FRAME_DATA_BIN] = dataBin;

    let binIndex = 0;
    let valueIndex = FRAME_DATA_BIN + 1;

    while (binIndex < 8) {
      values[valueIndex] = Number.parseInt(dataBin[binIndex]);

      valueIndex += 1;
      binIndex += 1;
    }
  };

  /**
   * @param {Event | null} event
   */
  const updateSignalMaskTable = (event) => {
    //console.debug('Updating signal mask table', event, canMessages);

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const { selectedRowId: signalId } = tableParams["dbc-can-signals-body"];

    if (!signalId) {
      //console.debug('No changes');

      return;
    }

    calculateSignalMasks();

    const { values: messageValues } = canMessages[messageId];
    const { values: signalValues, signalMask } = canMessages[messageId].signals[
      signalId
    ];
    const currentDlc = signalMaskBody.children.length;
    const dlc = messageValues[MESSAGE_DLC];

    if (currentDlc < dlc) {
      // Insert missing rows

      const newSignalMask = [];

      let diff = dlc - currentDlc;

      while (diff > 0) {
        const values = [...new Array(8).fill(0)];
        newSignalMask.push(values);

        diff -= 1;
      }

      insertRows(signalMaskBody, null, newSignalMask, false);
    } else if (dlc < currentDlc) {
      // Remove extra rows

      let diff = currentDlc - dlc;

      while (diff > 0) {
        signalMaskBody.lastElementChild.remove();

        diff -= 1;
      }
    }

    /** @type {HTMLTableRowElement[]} */
    const signalMaskRows = [...signalMaskBody.children];

    for (let i = 0, numRows = signalMaskRows.length; i < numRows; i++) {
      const { cells } = signalMaskRows[i];

      for (let j = 0, numCells = cells.length; j < numCells; j++) {
        cells[j].title = signalMask[i][j];
        cells[j].textContent = signalMask[i][j];
      }
    }

    colorBitIndicesTable();

    let finalDataBin = "";
    for (let i = 0, numRows = signalMask.length; i < numRows; i++) {
      const dataHex = canFrame[i][FRAME_DATA_HEX];
      const dataBin = hexToBin(dataHex, 8);
      for (let j = 0, numCols = signalMask[i].length; j < numCols; j++) {
        if (signalMask[i][j] === 1) {
          finalDataBin += dataBin[j];
        }
      }
    }

    const fullDataHex = binToHex(finalDataBin);
    const fullDataDec = hexToDec(fullDataHex);

    updateResult(fullDataHex, fullDataDec);
    updatePreview();
  };

  /**
   * Calculates the signal masks for all signals of the selected message and stores them.
   */
  const calculateSignalMasks = () => {
    //console.debug('Calculating signal masks', canMessages);

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const message = canMessages[messageId];
    const { values: messageValues, signals } = message;
    const dlc = messageValues[MESSAGE_DLC];

    for (const signal of Object.values(signals)) {
      signal.signalMask = new Array(dlc)
        .fill("")
        .map(() => [...new Array(8).fill(0)]);
      signal.activeBitIndices = [];
      signal.lsb = 0;
      signal.msb = 0;

      const { values: signalValues, signalMask } = signal;
      const isLittleEndian =
        ENDIAN_MAP[signalValues[SIGNAL_ORDER]] === "little";

      let bitStart = signalValues[SIGNAL_START];
      //insert code in here.
      if (!isLittleEndian) {
        bitStart = Math.floor(bitStart / 8) * 8 + 7 - (bitStart % 8);
      }
      //const bitStart = signalValues[SIGNAL_START];
      const bitLength = signalValues[SIGNAL_LENGTH];
      const numRows = signalMask.length;
      const lastRowIndex = numRows - 1;
      // Since the calculations use the next values, we do them backwards
      for (let i = lastRowIndex; i > -1; i--) {
        //const minBitIndex = Math.min(...bitIndices[i].slice(1));
        const minBitIndex = !isLittleEndian
          ? Math.min(...bitIndices[i].slice(1))
          : Math.min(...bitIndices[i].slice(1));

        //const maxBitIndex = Math.max(...bitIndices[i].slice(1));
        const maxBitIndex = !isLittleEndian
          ? Math.max(...bitIndices[i].slice(1))
          : Math.max(...bitIndices[i].slice(1));
        const nextRowMaxBit =
          i < lastRowIndex ? Math.max(...signalMask[i + 1].slice(0, 8)) : 0;
        const nextRowsBitSum =
          i < lastRowIndex
            ? signalMask
              .slice(i + 1, numRows)
              .map((row) => row.slice(0, 8))
              .flat()
              .reduce((prevValue, currValue) => prevValue + currValue, 0)
            : 0;

        for (let j = 7; j > -1; j--) {
          const bitIndex = isLittleEndian
            ? bitIndices[i].slice(1)[j]
            : bitIndices1[i].slice(1)[j];
          const nextColumnsBitSum =
            j < 7
              ? signalMask[i]
                .slice(j + 1, 8)
                .reduce((prevValue, currValue) => prevValue + currValue, 0)
              : 0;

          const isActive =
            bitIndex >= bitStart && bitIndex < bitStart + bitLength;
          // 	isLittleEndian && bitIndex >= bitStart && bitIndex < bitStart + bitLength) ||
          // (!isLittleEndian &&
          // 	(bitIndex === bitStart ||
          // 		(bitIndex > bitStart &&
          // 			minBitIndex <= bitStart &&
          // 			maxBitIndex >= bitStart &&
          // 			nextRowsBitSum + nextColumnsBitSum < bitLength) ||
          // 		(nextRowMaxBit === 1 && nextRowsBitSum + nextColumnsBitSum < bitLength)));
          const bit = isActive ? 1 : 0;
          signalMask[i][j] = bit;

          if (isActive) {
            signal.activeBitIndices.push(bitIndex);

            if (bitIndex === bitStart) {
              signal.lsb = bitIndex;
            }
          }
        }
      }

      signal.msb = calculateMsb(
        isLittleEndian,
        bitStart,
        bitLength,
        signalMask
      );

      if (
        signal.cells[SIGNAL_START].firstElementChild.validationMessage ===
        "Must not overlap other signals." ||
        signal.cells[SIGNAL_START].firstElementChild.validationMessage ===
        "Must not exceed message DLC."
      ) {
        signal.cells[SIGNAL_START].firstElementChild.setCustomValidity("");
        reportValidityIfFocused(signal.cells[SIGNAL_START].firstElementChild);
      }

      if (
        signal.cells[SIGNAL_LENGTH].firstElementChild.validationMessage ===
        "Must not overlap other signals." ||
        signal.cells[SIGNAL_LENGTH].firstElementChild.validationMessage ===
        "Must not exceed message DLC."
      ) {
        signal.cells[SIGNAL_LENGTH].firstElementChild.setCustomValidity("");
        reportValidityIfFocused(signal.cells[SIGNAL_LENGTH].firstElementChild);
      }

      checkRowErrorAttribute("can-signals", message.row, signal.row);
    }

    for (const signal of Object.values(signals)) {
      if (signal.activeBitIndices.length !== signal.values[SIGNAL_LENGTH]) {
        signal.cells[SIGNAL_START].firstElementChild.setCustomValidity(
          "Must not exceed message DLC."
        );
        signal.cells[SIGNAL_LENGTH].firstElementChild.setCustomValidity(
          "Must not exceed message DLC."
        );
        reportValidityIfFocused(signal.cells[SIGNAL_START].firstElementChild);
        reportValidityIfFocused(signal.cells[SIGNAL_LENGTH].firstElementChild);
        checkRowErrorAttribute("can-signals", message.row, signal.row);
      }

      if (signal.values[SIGNAL_MODE] !== "signal") {
        continue;
      }

      for (const currentSignal of Object.values(signals)) {
        if (
          signal === currentSignal ||
          currentSignal.values[SIGNAL_MODE] !== "signal"
        ) {
          continue;
        }

        if (
          signal.activeBitIndices.some((bit) =>
            currentSignal.activeBitIndices.includes(bit)
          )
        ) {
          signal.cells[SIGNAL_START].firstElementChild.setCustomValidity(
            "Must not overlap other signals."
          );
          signal.cells[SIGNAL_LENGTH].firstElementChild.setCustomValidity(
            "Must not overlap other signals."
          );
          reportValidityIfFocused(signal.cells[SIGNAL_START].firstElementChild);
          reportValidityIfFocused(
            signal.cells[SIGNAL_LENGTH].firstElementChild
          );
          checkRowErrorAttribute("can-signals", message.row, signal.row);
        }
      }
    }
  };

  const colorBitIndicesTable = () => {
    //console.debug('Coloring bit indices table');

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const { selectedRowId: selectedSignalId } = tableParams[
      "dbc-can-signals-body"
    ];

    if (!selectedSignalId) {
      //console.debug('No changes');

      return;
    }

    /** @type {HTMLTableRowElement[]} */
    const bitIndicesRows = [...bitIndicesBody.children];

    const numRows = bitIndicesRows.length;

    // Clear previous colors
    for (let i = 0; i < numRows; i++) {
      const { cells } = bitIndicesRows[i];

      for (let j = 1; j < 9; j++) {
        cells[j].style.backgroundColor = "";
        cells[j].style.color = "";
      }
    }

    const { signals } = canMessages[messageId];
    let k = 0;

    for (const [signalId, signal] of Object.entries(signals)) {
      const inactiveBitColor = [156 + k, 156 + k, 156 + k];

      for (let i = 0; i < numRows; i++) {
        const { cells } = bitIndicesRows[i];

        for (let j = 1; j < 9; j++) {
          if (cells[j].style.backgroundColor || cells[j].style.color) {
            continue;
          }

          //const bitIndex = (signal.values[SIGNAL_ORDER] === 'intel')? bitIndices[i][j]:bitIndices[i][j];
          const bitIndex =
            signal.values[SIGNAL_ORDER] === "intel"
              ? bitIndices[i][j]
              : bitIndices1[i][j];
          let bgColor;

          if (signalId === selectedSignalId) {
            if (bitIndex === signal.lsb) {
              bgColor = [61, 133, 198];
            } else if (bitIndex === signal.msb) {
              bgColor = [255, 153, 0];
            } else if (signal.activeBitIndices.includes(bitIndex)) {
              bgColor = [159, 197, 232];
            }
          } else if (signal.activeBitIndices.includes(bitIndex)) {
            bgColor = inactiveBitColor;
          }

          if (bgColor) {
            const [red, green, blue] = bgColor;
            let color;

            // Determine if the best text color for the background is either black or white
            if (red * 0.299 + green * 0.587 + blue * 0.114 > 186) {
              color = "#000";
            } else {
              color = "#fff";
            }

            cells[j].style.backgroundColor = `rgb(${bgColor.join(", ")})`;
            cells[j].style.color = color;
          }
        }
      }

      k += 4;
    }
  };

  /**
   * @param {string} fullDataHex
   * @param {number} fullDataDec
   */
  const updateResult = (fullDataHex, fullDataDec) => {
    //console.debug('Updating result', fullDataHex, fullDataDec);

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      //console.debug('No changes');

      return;
    }

    const { selectedRowId: signalId } = tableParams["dbc-can-signals-body"];

    if (!signalId) {
      //console.debug('No changes');

      return;
    }

    const { values } = canMessages[messageId].signals[signalId];

    const factor = values[SIGNAL_FACTOR];
    const offset = values[SIGNAL_OFFSET];
    const unit = values[SIGNAL_UNIT];
    const bitLength = values[SIGNAL_LENGTH];
    const signalType = values[SIGNAL_TYPE]; // Get the signal type (signed/unsigned)

    // Get references to the warning and preview containers
    const warningContainer = document.querySelector('#dbc-result-warning');
    const previewContainer = document.querySelector('#dbc-result-preview');

    // Check if bit length is >= 54 and show/hide appropriate elements
    if (bitLength >= 54) {
      // Show warning and hide preview for large bit lengths
      warningContainer.style.display = 'block';
      previewContainer.style.display = 'none';
      return;
    } else {
      // Hide warning and show preview for supported bit lengths
      warningContainer.style.display = 'none';
      previewContainer.style.display = 'block';
    }

    // Handle signed values correctly
    let adjustedDataDec = fullDataDec;
    if (signalType === "sig") { // If the signal is signed
      // Check if the most significant bit is set (indicates a negative value)
      const fullDataBin = decToBin(fullDataDec, bitLength);
      if (fullDataBin.charAt(0) === "1") {
        // Calculate the two's complement to get the negative value
        // 2^bitLength is the maximum value + 1 for this bit length
        adjustedDataDec = fullDataDec - Math.pow(2, bitLength);
      }
    } else {
      // For unsigned values, use the original value
      adjustedDataDec = fullDataDec;
    }

    // Calculate the physical value with the correct decimal value
    const result = factor * adjustedDataDec + offset;

    // Convert decimal to binary for display with proper bit length padding
    const fullDataBin = decToBin(fullDataDec, bitLength);

    // Update all result displays
    resultDataBin.textContent = fullDataBin;
    resultDataHex.textContent = fullDataHex;
    resultDataDec.textContent = fullDataDec.toString();
    resultValueFactor.textContent = factor.toString();
    resultValueDec.textContent = adjustedDataDec.toString(); // Use the adjusted value
    resultValueOffset.textContent = offset.toString();
    resultValue.textContent = result.toString();
    resultValueUnit.textContent = unit;
  };

  const updatePreview = () => {
    //console.debug('Updating preview');

    const { selectedRowId: messageId } = tableParams["dbc-can-messages-body"];

    if (!messageId) {
      preview.textContent = "";

      return;
    }

    const message = canMessages[messageId];
    const { values: messageValues, signals } = message;
    const messageName = messageValues[MESSAGE_NAME];

    if (!messageName) {
      // Ignore messages with no name

      preview.textContent = "";

      return;
    }

    const lines = [];

    lines.push(generateMessageLine(message));

    for (const signal of Object.values(signals)) {
      const { values: signalValues } = signal;

      const signalName = signalValues[SIGNAL_NAME];

      if (!signalName) {
        // Ignore signals with no name

        continue;
      }

      lines.push(generateSignalLine(message, signal));
    }

    preview.textContent = lines.join("\n");
  };

  /**
   * @param {boolean} isLittleEndian
   * @param {number} bitStart
   * @param {number} bitLength
   * @param {unknown[][]} signalMask
   * @returns {number}
   */
  const calculateMsb = (isLittleEndian, bitStart, bitLength, signalMask) => {
    //console.debug('Calculating MSB', isLittleEndian, bitStart, bitLength, signalMask);

    // if (isLittleEndian) {
    // 	return bitStart + bitLength - 1;
    // }

    // let msbSum = 0;

    // for (let i = 0, numRows = signalMask.length; i < numRows; i++) {
    // 	const prevRowMaxBit = i > 0 ? Math.max(...signalMask[i - 1].slice(0, 8)) : 0;

    // 	for (let j = 0; j < 8; j++) {
    // 		const bit = signalMask[i][j];
    // 		const prevBit = j > 0 ? signalMask[i][j - 1] : 0;
    // 		const nextBit = j < 7 ? signalMask[i][j + 1] : 0;

    // 		msbSum +=
    // 			prevRowMaxBit === 0 && bit === 1 && prevBit !== 1 && nextBit === 1
    // 				? bitIndices[i].slice(1)[j]
    // 				: 0;
    // 	}
    // }

    return bitStart + bitLength - 1;
  };

  const generateMessageLine = (message) => {
    //console.debug('Generating message line', message);

    const canId = message.values[MESSAGE_CAN_ID];
    const dbcIdDec = canIdToDbcId(
      canId,
      message.values[MESSAGE_TYPE],
      currNumberFormat,
      "dec"
    );
    const name = message.values[MESSAGE_NAME];
    const dlc = message.values[MESSAGE_DLC];

    return `BO_ ${dbcIdDec} ${name}: ${dlc} ${message.line.values.sender || "Vector__XXX"
      }`;
  };

  const generateSignalLine = (message, signal) => {
    //console.debug('Generatig signal line', message, signal);

    const name = signal.values[SIGNAL_NAME];
    const type = signal.values[SIGNAL_TYPE];
    const isLittleEndian = ENDIAN_MAP[signal.values[SIGNAL_ORDER]] === "little";
    const mode = signal.values[SIGNAL_MODE];
    const multiplexor = signal.values[SIGNAL_MULTIPLEXOR];
    const multiplexValue = signal.values[SIGNAL_MULTIPLEX_VALUE];
    const bitStart = signal.values[SIGNAL_START];
    const bitLength = signal.values[SIGNAL_LENGTH];
    const factor = signal.values[SIGNAL_FACTOR];
    const offset = signal.values[SIGNAL_OFFSET];
    const min = signal.values[SIGNAL_MIN];
    const max = signal.values[SIGNAL_MAX];
    const unit = signal.values[SIGNAL_UNIT];

    const isMultiplexorRoot =
      multiplexor !== "None" &&
      message.signals[multiplexor] &&
      message.signals[multiplexor].values[SIGNAL_MODE] === "multiplexor";

    return [
      `${signalIndentation}SG_ ${name}`,
      mode === "multiplexor" || mode === "multiplexingGroup"
        ? ` ${mode === "multiplexingGroup" ? `m${multiplexValue}` : ""}${mode === "multiplexor" || isMultiplexorRoot ? "M" : ""
        }`
        : "",
      " : ",
      isLittleEndian ? bitStart : bitStart,
      "|",
      bitLength,
      isLittleEndian ? "@1" : "@0",
      type === "unsig" ? "+" : "-",
      ` (${factor},${offset})`,
      ` [${min}|${max}]`,
      ` "${unit}"`,
      ` ${signal.line.values.receiver || "Vector__XXX"}`
    ].join("");
  };

  const generateMessageCommentLine = (message) => {
    //console.debug('Generating message comment line', message);

    const comment = message.values[MESSAGE_COMMENT];

    if (!comment) {
      return "";
    }

    const canId = message.values[MESSAGE_CAN_ID];
    const dbcIdDec = canIdToDbcId(
      canId,
      message.values[MESSAGE_TYPE],
      currNumberFormat,
      "dec"
    );

    return `CM_ BO_ ${dbcIdDec} "${comment}";`;
  };

  const generateMessageTypeLine = (message) => {
    //console.debug('Generating message type line', message);

    const type = message.values[MESSAGE_TYPE];
    //console.log(type)

    if (type != "extendedJ1939PG") {
      return "";
    }

    const canId = message.values[MESSAGE_CAN_ID];
    const dbcIdDec = canIdToDbcId(
      canId,
      message.values[MESSAGE_TYPE],
      currNumberFormat,
      "dec"
    );
    console.log("return message - BA_ VFrameFormat BO_ ' " + dbcIdDec + "' 3");
    return `BA_ "VFrameFormat" BO_ ${dbcIdDec} 3;`;
  };

  const generateSignalCommentLine = (message, signal) => {
    //console.debug('Generatig signal comment line', message, signal);

    const comment = signal.values[SIGNAL_COMMENT];

    if (!comment) {
      return "";
    }

    const canId = message.values[MESSAGE_CAN_ID];
    const dbcIdDec = canIdToDbcId(
      canId,
      message.values[MESSAGE_TYPE],
      currNumberFormat,
      "dec"
    );
    const name = signal.values[SIGNAL_NAME];

    return `CM_ SG_ ${dbcIdDec} ${name} "${comment}";`;
  };

  const generateSignalMultiplexLine = (message, signal) => {
    //console.debug('Generating signal multiplex line', message, signal);

    const canId = message.values[MESSAGE_CAN_ID];
    const dbcIdDec = canIdToDbcId(
      canId,
      message.values[MESSAGE_TYPE],
      currNumberFormat,
      "dec"
    );

    const name = signal.values[SIGNAL_NAME];
    const multiplexor = signal.values[SIGNAL_MULTIPLEXOR];
    const multiplexValue = signal.values[SIGNAL_MULTIPLEX_VALUE];

    return `SG_MUL_VAL_ ${dbcIdDec} ${name} ${multiplexor} ${multiplexValue}-${multiplexValue};`;
  };

  /**
   * @type {CanIdToDbcId}
   */
  const canIdToDbcId = (
    canId,
    type,
    sourceFormat = currNumberFormat,
    targetFormat = currNumberFormat
  ) => {
    let dbcIdDec = formatNumber(canId, sourceFormat, "dec");

    if (type === "extended" || type === "extendedJ1939PG") {
      dbcIdDec = (dbcIdDec | (4 << 29)) >>> 0;
    }

    return formatNumber(dbcIdDec, "dec", targetFormat);
  };

  /**
   * @type {DbcIdToCanId}
   */
  const dbcIdToCanId = (
    dbcId,
    type,
    sourceFormat = currNumberFormat,
    targetFormat = currNumberFormat
  ) => {
    let canIdDec = formatNumber(dbcId, sourceFormat, "dec");

    if (type === "extended" || type === "extendedJ1939PG") {
      canIdDec = (canIdDec & 0x1fffffff) >>> 0;
    }

    return formatNumber(canIdDec, "dec", targetFormat);
  };

  /**
   * Takes a number or string in a `sourceFormat` and converts it to a `targetFormat`, between decimal and hexadecimal.
   *
   * @type {FormatNumber}
   */
  const formatNumber = (
    num,
    sourceFormat = prevNumberFormat,
    targetFormat = currNumberFormat,
    size
  ) => {
    if (sourceFormat === "dec") {
      const dec = typeof num === "string" ? strToDec(num) : num;

      return targetFormat === "hex" ? decToHex(dec, size) : dec;
    }

    return targetFormat === "dec" ? hexToDec(num) : num;
  };

  /**
   * @param {string} bin
   * @returns {number}
   */
  const binToDec = (bin) => {
    return Number.parseInt(bin, 2);
  };

  /**
   * @param {number} dec
   * @param {number} [size]
   * @returns {string}
   */
  const decToBin = (dec, size) => {
    if (size) {
      return `${"0".repeat(size)}${dec.toString(2)}`.slice(-size);
    }

    return dec.toString(2);
  };

  /**
   * @param {string} hex
   * @returns {number}
   */
  const hexToDec = (hex) => {
    return Number.parseInt(hex, 16);
  };

  /**
   * @param {number} dec
   * @param {number} [size]
   * @returns {string}
   */
  const decToHex = (dec, size) => {
    if (size) {
      return `${"0".repeat(size)}${dec.toString(16).toUpperCase()}`.slice(
        -size
      );
    }

    return dec.toString(16).toUpperCase();
  };

  /**
   * @param {string} bin
   * @returns {number}
   */
  const binToHex = (bin) => {
    return decToHex(binToDec(bin));
  };

  /**
   * @param {string} hex
   * @param {number} [size]
   * @returns {string}
   */
  const hexToBin = (hex, size) => {
    return decToBin(hexToDec(hex), size);
  };

  /**
   * @param {string} str
   * @returns {number}
   */
  const strToDec = (str) => {
    return Number.parseInt(str, 10);
  };

  /**
   * INITIALIZATION
   */

  tableParams = {
    "dbc-can-messages-body": {
      tableId: "can-messages",
      columns: [
        {
          type: "text"
        },
        {
          type: "radio"
        },
        {
          type: "input",
          name: "name",
          attributes: {
            maxlength: 32
          },
          isIdColumn: true,

          validate(input) {
            const value = input.value;

            if (value.length < 1 || value.length > 32) {
              return "Must be 1-32 characters.";
            }

            if (!/^\w*$/.test(value)) {
              return "Must contain only letters, digits and underscores.";
            }

            /** @type {HTMLTableRowElement} */
            const row = input.parentElement.parentElement;

            const rowId = row.getAttribute("data-id");
            const id = value;

            if (id !== rowId && canMessages[id]) {
              return "Must be unique.";
            }

            return "";
          }
        },
        {
          type: "input",
          name: "can-id",

          validate(input, message) {
            const value = input.value;

            if (currNumberFormat === "hex" && !/^[A-Fa-f0-9]*$/.test(value)) {
              return "Must be a valid hexadecimal.";
            }

            if (currNumberFormat === "dec" && !/^[0-9]*$/.test(value)) {
              return "Must be a valid decimal.";
            }

            const dec = formatNumber(value, currNumberFormat, "dec");

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            const type = message?.values[MESSAGE_TYPE] || "standard";

            if (type === "standard" && dec > 0x7ff) {
              return "Must be 1-11 bits.";
            }

            if (type !== "standard" && dec > 0x1fffffff) {
              return "Must be 1-29 bits.";
            }

            return "";
          }
        },
        {
          type: "select",
          name: "type",
          options: {
            standard: "Standard",
            extended: "Extended",
            extendedJ1939PG: "Extended (J1939PG)"
          },
          defaultOption: "standard",
          dependencies: [MESSAGE_CAN_ID]
        },
        {
          type: "input",
          name: "dlc",
          attributes: {
            type: "number",
            min: 0,
            max: 2000,
            step: 1
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseInt(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec < 0 || dec > 1785) {
              return "Must be between 0 and 1785.";
            }

            return "";
          },

          transform(value) {
            return Number.parseInt(value);
          }
        },
        {
          type: "input",
          name: "comment",
          attributes: {
            maxlength: 255
          },

          validate(input) {
            const value = input.value;

            if (value.length > 255) {
              return "Must have 0-255 characters.";
            }

            return "";
          }
        }
      ],
      fieldColumns: [],
      defaultValues: ["!", "", "", "", "standard", 0, ""],
      isEditable: true,
      selectedRow: null,
      selectedRowId: null,
      updateRowValues: updateCanMessagesRowValues
    },
    "dbc-can-signals-body": {
      tableId: "can-signals",
      columns: [
        {
          type: "text"
        },
        {
          type: "radio"
        },
        {
          type: "input",
          name: "name",
          attributes: {
            maxlength: 32
          },
          isIdColumn: true,

          validate(input) {
            const value = input.value;

            if (value.length < 1 || value.length > 32) {
              return "Must be 1-32 characters.";
            }

            if (!/^\w*$/.test(value)) {
              return "Must contain only letters, digits and underscores.";
            }

            /** @type {HTMLTableRowElement} */
            const row = input.parentElement.parentElement;

            const { selectedRowId: messageId } = tableParams[
              "dbc-can-messages-body"
            ];
            const rowId = row.getAttribute("data-id");
            const id = value;

            if (id !== rowId && canMessages[messageId].signals[id]) {
              return "Must be unique.";
            }

            return "";
          }
        },
        {
          type: "select",
          name: "type",
          options: {
            unsig: "Unsigned",
            sig: "Signed"
          },
          defaultOption: "unsig"
        },
        {
          type: "select",
          name: "order",
          options: {
            intel: "Intel",
            motorola: "Motorola"
          },
          defaultOption: "intel"
        },
        {
          type: "select",
          name: "mode",
          options: {
            signal: "Signal",
            multiplexor: "Multiplexor",
            multiplexingGroup: "Multiplexing group"
          },
          defaultOption: "signal"
        },
        {
          type: "select",
          name: "multiplexor",
          options: {
            None: "None"
          },
          defaultOption: "None"
        },
        {
          type: "input",
          name: "multiplexValue",
          attributes: {
            type: "number",
            step: 1
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseInt(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            return "";
          },

          transform(value) {
            return Number.parseInt(value);
          }
        },
        {
          type: "input",
          name: "start",
          attributes: {
            type: "number",
            min: 0,
            step: 1
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseInt(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec < 0) {
              return "Must be positive.";
            }

            return "";
          },

          transform(value) {
            return Number.parseInt(value);
          }
        },
        {
          type: "input",
          name: "length",
          attributes: {
            type: "number",
            min: 0,
            step: 1
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseInt(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec < 0) {
              return "Must be positive.";
            }

            return "";
          },

          transform(value) {
            return Number.parseInt(value);
          }
        },
        {
          type: "input",
          name: "factor",
          attributes: {
            type: "number",
            step: "any"
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseFloat(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            return "";
          },

          transform(value) {
            return Number.parseFloat(value);
          }
        },
        {
          type: "input",
          name: "offset",
          attributes: {
            type: "number",
            step: "any"
          },

          validate(input) {
            const value = input.value;
            const dec = Number.parseFloat(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            return "";
          },

          transform(value) {
            return Number.parseFloat(value);
          }
        },
        {
          type: "input",
          name: "min",
          attributes: {
            type: "number",
            step: "any"
          },

          validate(input, message, signal) {
            const value = input.value;
            const dec = Number.parseFloat(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec > signal.values[SIGNAL_MAX]) {
              signal.cells[SIGNAL_MAX].firstElementChild.setCustomValidity(
                "Must be bigger than Min."
              );
              reportValidityIfFocused(
                signal.cells[SIGNAL_MAX].firstElementChild
              );

              return "Must be smaller than Max.";
            }

            signal.cells[SIGNAL_MAX].firstElementChild.setCustomValidity("");
            reportValidityIfFocused(signal.cells[SIGNAL_MAX].firstElementChild);

            return "";
          },

          transform(value) {
            return Number.parseFloat(value);
          }
        },
        {
          type: "input",
          name: "max",
          attributes: {
            type: "number",
            step: "any"
          },

          validate(input, message, signal) {
            const value = input.value;
            const dec = Number.parseFloat(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec < signal.values[SIGNAL_MIN]) {
              signal.cells[SIGNAL_MIN].firstElementChild.setCustomValidity(
                "Must be smaller than Max."
              );
              reportValidityIfFocused(
                signal.cells[SIGNAL_MIN].firstElementChild
              );

              return "Must be bigger than Min.";
            }

            signal.cells[SIGNAL_MIN].firstElementChild.setCustomValidity("");
            reportValidityIfFocused(signal.cells[SIGNAL_MIN].firstElementChild);

            return "";
          },

          transform(value) {
            return Number.parseFloat(value);
          }
        },
        {
          type: "input",
          name: "unit"
        },
        {
          type: "input",
          name: "comment",
          attributes: {
            maxlength: 255
          },

          validate(input) {
            const value = input.value;

            if (value.length > 255) {
              return "Must have 0-255 characters.";
            }

            return "";
          }
        }
      ],
      fieldColumns: [],
      defaultValues: [
        "!",
        "",
        "",
        "unsig",
        "intel",
        "signal",
        "None",
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        "",
        ""
      ],
      isEditable: true,
      selectedRow: null,
      selectedRowId: null,
      updateRowValues: updateCanSignalsRowValues
    },
    "dbc-can-frame-body": {
      tableId: "can-frame",
      columns: [
        {
          type: "text"
        },
        {
          type: "input",
          name: "data-hex",
          attributes: {
            maxlength: 2
          },

          validate(input) {
            const value = input.value;

            if (!/^[A-Fa-f0-9]*$/.test(value)) {
              return "Must be a valid hexadecimal.";
            }

            const dec = hexToDec(value);

            if (Number.isNaN(dec)) {
              return "Must be a valid number.";
            }

            if (dec < 0 || dec > 0xff) {
              return "Must be between 00 and FF.";
            }

            return "";
          }
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        }
      ],
      fieldColumns: [],
      defaultValues: [],
      isEditable: false,
      selectedRow: null,
      selectedRowId: null,
      updateRowValues: updateCanFrameRowValues
    },
    "dbc-bit-indices-body": {
      tableId: "bit-indices",
      columns: [
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        }
      ],
      fieldColumns: [],
      defaultValues: [],
      isEditable: false,
      selectedRow: null,
      selectedRowId: null,
      updateRowValues: null
    },
    "dbc-signal-mask-body": {
      tableId: "signal-mask",
      columns: [
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        },
        {
          type: "text"
        }
      ],
      fieldColumns: [],
      defaultValues: [],
      isEditable: false,
      selectedRow: null,
      selectedRowId: null,
      updateRowValues: null
    }
  };

  // Fill field columns
  for (const params of Object.values(tableParams)) {
    const { columns } = params;
    params.fieldColumns = [...columns.keys()].filter((i) => {
      const column = columns[i];

      return column.type === "input" || column.type === "select";
    });
  }

  document.addEventListener("click", checkClick);
  //updateTables(event);
  //document.addEventListener('load', updateTables);
  openButton.addEventListener("click", openFileDialog);
  downloadButton.addEventListener("click", openDiffModal);
  fileInput.addEventListener("change", loadFile);
  modalClose.addEventListener("click", closeModal);

  numberFormatSelect.addEventListener("change", changeNumberFormat);

  canMessagesBody.addEventListener("keydown", checkFieldKey);
  canMessagesBody.addEventListener("input", updateRow);
  canMessagesBody.addEventListener("change", updateRow);

  // canSignalsBody.addEventListener('keydown', checkFieldKey);
  // canSignalsBody.addEventListener('input', updateRow);
  canSignalsBody.addEventListener("change", updateRow);

  canFrameBody.addEventListener("keydown", checkFieldKey);
  canFrameBody.addEventListener("input", updateRow);
  canFrameBody.addEventListener("change", updateRow);

  // Set default values
  parseFile();
})();


