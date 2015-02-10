/* Room names */
CREATE TABLE tblRoom (
    roomName text PRIMARY KEY
);
STATEMENT_END

/* Device type (Lamp, temperature sensor, etc) */
CREATE TABLE tblType (
    deviceName text PRIMARY KEY
    deviceType text NOT NULL,
);
STATEMENT_END

/* List of outputs devices connected to the house */
CREATE TABLE tblOutput (
    outputID text PRIMARY KEY,
    outputName text NOT NULL,
    fk_roomName text  NOT NULL,
    fk_deviceName text NOT NULL
);
STATEMENT_END

/* List of inputs devices connected to the house */
CREATE TABLE tblInput (
    inputID text PRIMARY KEY,
    inputName text NOT NULL,
    fk_roomName text  NOT NULL,
    fk_deviceName text NOT NULL
);
STATEMENT_END

/* Outputs states */
CREATE TABLE tblOutputState (
    fk_outputID text PRIMARY KEY,
    state bool NOT NULL
);
STATEMENT_END

/* Inputs states */
CREATE TABLE tblInputState (
    fk_inputID text PRIMARY KEY,
    val int NOT NULL
);
STATEMENT_END
