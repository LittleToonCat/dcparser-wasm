Module['vectorToUint8Array'] = (vector) => {
  const array = new Uint8Array(vector.size())
  for (let i = 0; i < vector.size(); i++) {
    array[i] = vector.get(i);
  }
  return array;
}

Module['vectorToBuffer'] = (vector) => {
  if (!ENVIRONMENT_IS_NODE) {
    throw new Error("vectorToBuffer can only be called within Node.");
  }
  const { Buffer } = require('buffer');

  const buffer = Buffer.from(Module['vectorToUint8Array'](vector));
  return buffer;
}

Module['vectorFromUint8Array'] = (array) => {
  // Also supports Node's Buffer.
  const vector = new Module.vector_uchar();

  for (let i = 0; i < array.length; i++) {
    const element = array[i];
    vector.push_back(element);
  }

  return vector;
}

Module['packObject'] = (dcPacker, object) => {
  // From https://dev.to/gladchinda/javascript-tip-whatis-a-better-typeof-3g0o
  function whatis (value) {
    return Object.prototype.toString.call(value)
      .replace(/^\[object\s+([a-z]+)\]$/i, '$1')
      .toLowerCase();
  }

  switch (dcPacker.packType.value) {
    case Module.DCPackType.PTInt64.value:
      if (['number', 'bigint'].includes(whatis(object))) {
        dcPacker.packInt64(object);
        return;
      }
      break;

    case Module.DCPackType.PTUInt64.value:
      if (['number', 'bigint'].includes(whatis(object))) {
        dcPacker.packUint64(object);
        return;
      }
      break;

    case Module.DCPackType.PTInt.value:
      if (whatis(object) == 'number') {
        dcPacker.packInt(object);
        return;
      }
      break;

    case Module.DCPackType.PTUint.value:
      if (whatis(object) == 'number') {
        dcPacker.packUint(object);
        return;
      }
      break;

    default:
      break;
  }

  const typeOfObject = whatis(object);
  if (typeOfObject == 'number') {
    if (Number.isInteger(object)) {
      dcPacker.packInt(object);
    } else {
      // is float
      dcPacker.packDouble(object);
    }
  } else if (typeOfObject == 'bigint') {
    dcPacker.packInt64(object);
  } else if (typeOfObject == 'string') {
    dcPacker.packString(object);
  } else if (typeOfObject == '[object uint8array]') {
    const vector = Module.vectorFromUint8Array(object);
    dcPacker.packBlob(vector);
    vector.delete()
  } else if (typeOfObject == 'array') {
    dcPacker.push();

    for (let i = 0; i < object.length; i++) {
      const element = object[i];
      Module.packObject(dcPacker, element);
    }
    dcPacker.pop();
  } else {
    throw new TypeError(`Don't know how to pack object: ${object}`);
  }
}

Module['unpackObject'] = (dcPacker) => {
  let object = null;
  switch (dcPacker.packType.value) {
    case Module.DCPackType.PTInvalid.value:
      dcPacker.unpackSkip();
      break;
    case Module.DCPackType.PTDouble.value:
      object = dcPacker.unpackDouble();
      break;
    case Module.DCPackType.PTInt.value:
      object = dcPacker.unpackInt();
      break;
    case Module.DCPackType.PTUint.value:
      object = dcPacker.unpackUint();
      break;
    case Module.DCPackType.PTInt64.value:
      object = dcPacker.unpackInt64();
      break;
    case Module.DCPackType.PTUInt64.value:
      object = dcPacker.unpackUint64();
      break;
    case Module.DCPackType.PTBlob.value:
      if (ENVIRONMENT_IS_NODE) {
        object = Module.vectorToBuffer(dcPacker.unpackBlob());
      } else {
        object = Module.vectorToUint8Array(dcPacker.unpackBlob());
      }
      break;
    case Module.DCPackType.PTString.value:
      object = dcPacker.unpackString();
      break;
    default: {
      object = [];

      dcPacker.push();
      while(dcPacker.moreNestedFields) {
        const element = Module.unpackObject(dcPacker);
        object.push(element);
      }
      dcPacker.pop();
      break;
    }
  }

  return object;
}

Module['packField'] = (dcField, elements) => {
  const dcPacker = new Module.DCPacker()
  let data = null
  try {
    dcPacker.beginPack(dcField);
    Module.packObject(dcPacker, elements);
    if (!dcPacker.endPack()) {
      throw new Error("Unknown error");
    }
    if (ENVIRONMENT_IS_NODE) {
      data = Module.vectorToBuffer(dcPacker.data)
    } else {
      data = Module.vectorToUint8Array(dcPacker.data);
    }
  } catch (error) {
    console.log(`Error has occured when attempting to pack field "${dcField.name}": ${error}`);
    data = error;
  } finally {
    // Clean up the packer itself.
    dcPacker.delete();
    return data;
  }
}

Module['unpackField'] = (dcField, data) => {
  // From https://dev.to/gladchinda/javascript-tip-whatis-a-better-typeof-3g0o
  function whatis (value) {
    return Object.prototype.toString.call(value)
      .replace(/^\[object\s+([a-z]+)\]$/i, '$1')
      .toLowerCase();
  }

  if (whatis(data) == '[object uint8array]') {
    // convert into a vector_uchar
    const vector = Module.vectorFromUint8Array(data);
    data = vector;
  }

  const dcPacker = new Module.DCPacker()
  let object = null;
  try {
    dcPacker.setUnpackData(data);
    dcPacker.beginUnpack(dcField);

    object = Module.unpackObject(dcPacker);

  } catch (error) {
    object = error;
  } finally {
    dcPacker.delete()
    data.delete();

    return object;
  }
}
