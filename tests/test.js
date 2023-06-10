"use strict";
const dcparser = require('../dcparser.js')

// Mount FS
dcparser.FS.mkdir('/dclass')
dcparser.FS.mount(dcparser.NODEFS, {root: './tests/'}, '/dclass')

dcparser['onRuntimeInitialized'] = () => {
    const dcFile = new dcparser.DCFile();

    if (dcFile.read('/dclass/sample.dc')) {
        console.log('Hash of dc file: %d', dcFile.hash)
    }

    const dcClass = dcFile.getClassByName('DistributedAvatar')
    const dcField = dcClass.getField(0)

    console.log("Testing pack")
    const elements = [['a', 1, 2, 3,[1, 0, 1, 0, 1], 1, 1]]
    const packedData = dcparser.packField(dcField, elements);
    console.log("Pack result:", packedData);

    console.log("Testing unpack");
    const unpackResult = dcparser.unpackField(dcField, packedData);
    console.log("Unpack result:", unpackResult);
}
