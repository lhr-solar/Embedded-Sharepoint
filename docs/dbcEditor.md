# Editing CAN DBC files

## What is a CAN DBC file
A CAN DBC file stores encodings for CAN messages. CAN messages are grouped into IDs with a length, and then messages have specific signals which have a length and start bit. A CAN message signal stores how a value is packed into raw CAN data, including its bit position, size, scaling factor, offset, and units.  

For example, we have Contactor Sense CAN message with an ID of `0x400`. There are various signals in this message like the state of the Array Contactor, which has a length of 1 bit and stored in the 6th bit.

[CAN DBC intro](https://www.csselectronics.com/pages/can-dbc-file-database-intro)

## How to edit CAN DBC files
To edit CAN DBC files, we use an online [DBC editor](https://www.csselectronics.com/pages/dbc-editor-can-bus-database) from CSS Electronics. We also forked the files to Embedded-Sharepoint in the `middleware/dbc-editor` directory.  

To open the CAN DBC editor you can do one of the following:

* Open the CAN DBC editor website linked above
* Open the `dbc-editor.html` file located in `middleware/dbc-editor`
* Run `make dbc-editor` in the Makefile in `tests/`

From the DBC editor you can open and edit any dbc file, all of which are stored in the `can/dbc` directory.

Ensure that your CAN message type is set to standard, and the length of the message (DLC) is not above 8 bytes.

## Suggestions for structuring CAN messages
When structuring CAN messages, consider which other devices on the CAN bus are using that message. 

### CAN message packing
It is usually beneficial to pack as much related data as possible into a single CAN message. Devices on the bus are often limited by the number of CAN IDs they can store in hardware filters, so reducing the total number of unique message IDs helps avoid exhausting filter slots.

As long as the message length remains under 8 bytes (for Classical CAN), there is little downside to using more bytes in a single frame.

### CAN multiplexing
CAN message multiplexing allows multiple logical message layouts to share the same CAN ID. A **multiplexor signal** is used to indicate which set of signals is valid for a given frame.

For example, instead of having an ID for every battery module's voltage, you can allocate a set of bits to indicate which module the message is about.
