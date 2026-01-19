# Editing CAN DBC files

## What is a CAN DBC file
A CAN DBC file stores encodings for CAN messages. CAN messages are grouped into IDs with a length, and then messages have specific signals which have a length and start bit. A CAN message signal stores how a value is packed into raw CAN data, including its bit position, size, scaling factor, offset, and units.  

For example, we have Contactor Sense CAN message with an ID of `0x400`. There are various signals in this message like the state of the Array Contactor, which has a length of 1 bit and stored in the 6th bit.

[CAN DBC intro](https://www.csselectronics.com/pages/can-dbc-file-database-intro)

## How to edit CAN DBC files
To edit CAN DBC files, we use an online [DBC editor](https://www.csselectronics.com/pages/dbc-editor-can-bus-database) from CSS Electronics. We also forked the files to Embedded-Sharepoint in the `middleware/dbc-editor` directory.  

To open the CAN DBC editor you can do one of the following:

* Open the CAN DBC editor website linked above
* Open the `dbc-editor.html` file located in `Embedded-Sharepoint/middleware/dbc-editor`
* Run `make dbc-editor` in the Makefile in `tests/`

## Suggestions for structuring CAN messages
