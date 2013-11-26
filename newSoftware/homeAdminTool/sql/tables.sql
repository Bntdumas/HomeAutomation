-- Room names
CREATE TABLE tblRoom (
    roomName text PRIMARY KEY
);
STATEMENT_END

-- Device type (Lamp, temperature sensor, etc)
CREATE TABLE tblType (
    deviceName text PRIMARY KEY
);
STATEMENT_END

-- List of outputs devices connected to the house
CREATE TABLE tblOutput (
    outputID uuid PRIMARY KEY,
    outputName text NOT NULL
    fk_roomName int  NOT NULL,
    fk_deviceName int NOT NULL
);
STATEMENT_END

-- List of inputs devices connected to the house
CREATE TABLE tblInput (
    inputID uuid PRIMARY KEY,
    inputName text NOT NULL
    fk_roomName int  NOT NULL,
    fk_deviceName int NOT NULL
);
STATEMENT_END

-- Outputs states
CREATE TABLE tblOutputState (
    outputStateID uuid PRIMARY KEY,
    state bool NOT NULL
);
STATEMENT_END

-- Inputs states
CREATE TABLE tblInputState (
    inputStateID uuid PRIMARY KEY,
    val int NOT NULL
);
STATEMENT_END
