"""Bounded SPB1 phase containers; filenames retain the native reader aliases."""
import struct

LIMIT = 2 * 1024 * 1024


def pack(files):
    assert 0 < len(files) <= 96
    offset = 16 + 40 * len(files)
    directory, payload = bytearray(), bytearray()
    for name, body in sorted(files.items()):
        encoded = name.encode('ascii')
        assert 0 < len(encoded) <= 30 and '/' not in name and body
        directory += encoded.ljust(32, b'\0') + struct.pack('>II', offset, len(body))
        payload += body
        offset += len(body)
    assert offset <= LIMIT, 'phase exceeds explicit 2-MiB bank limit'
    return b'SPB1' + struct.pack('>III', len(files), offset, 0) + directory + payload


def unpack(body):
    assert 16 <= len(body) <= LIMIT and body[:4] == b'SPB1'
    count, size, reserved = struct.unpack_from('>III', body, 4)
    assert 0 < count <= 96 and size == len(body) and reserved == 0
    end = 16 + count * 40
    assert end <= size
    files, previous = {}, ''
    for i in range(count):
        entry = body[16+i*40:56+i*40]
        assert b'\0' in entry[:32]
        name = entry[:32].split(b'\0')[0].decode('ascii')
        offset, length = struct.unpack_from('>II', entry, 32)
        assert name > previous and offset == end and 0 < length <= size-end
        files[name] = body[offset:offset+length]
        previous, end = name, offset+length
    assert end == size
    return files
