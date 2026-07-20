# UnrealCsvOutputPlugin

## Installation

Download the zip or clone this repository into your Plugins/ directory.
This plugin also depends on GetNamo's wonderful socketio-client-ue4-plugin here: https://github.com/getnamo/socketio-client-ue4.
Make sure you download and install that plugin to your project's directory as well.

Alternatively, if you like using submodules, use something like the below:

```
git submodule add https://github.com/getnamo/socketio-client-ue4 Plugins/SocketIoClient
git submodule add https://github.com/calben/UnrealCsvOutputPlugin Plugins/CsvOutput
```

## Usage

Csv entries are either headers or rows.
A header is the first line expected in a Csv file giving all the variables names.
Rows are for the values to be associated with those variables.

The values for your Csv file are alphabetically ordered by their name to ensure the correct order.
If you change the names of the variables in your struct at runtime, your variables will no longer be in the correct order.
If you want to do anything like change variable names at runtime (not sure why you'd want to, but you do you, man), you should use Json files to track your data.

A CsvHeader or CsvRow `FString` can be retrieved from a variable at any time using the `StructToCsvRow` or `StructToCsvHeader` pure Blueprints.
If you call these from C++, the "from Blueprints" bool should be set to false as no trimming will need to occur on the variable names.

If you want to track variables on a particular actor, add a `StructToCsvDumpComponent` to the actor and specify the component's file name and file directory to which it will dump.
To add the header to the Csv file to be dumped, called the `InitialiseStructToCsvFileHandle` function on the component; usually this would be done in the `BeginPlay` for the actor.
To add lines, call the `AddRowToStructToCsvFileHandle`; usually you would do this either in the tick of the actor for the highest resolution possible or on a custom event called with a timer.
After you're done adding lines to the buffer, flush it with `FlushBufferToFile` to write everything out to your Csv file.
Depending on how long the buffer is, this could cause performance issues.
If you want to destroy an actor that owns the component, make sure to either keep the component alive or flush the buffer.
