# Copyright (c) 2017-2020, Hans Erik Thrane

""" Test flatbuffers encoding/decoding works """

import flatbuffers

from roq.fbs import ConnectionStatus, DownloadBegin

def test_flatbuffers_simple():
    """ Simplest test case """
    disconnected = ConnectionStatus.ConnectionStatus().Disconnected
    connected = ConnectionStatus.ConnectionStatus().Connected
    assert disconnected != connected


def test_flatbuffers_first():
    """ First test case """
    # encode
    builder = flatbuffers.Builder(1024)
    account = builder.CreateString('test')
    DownloadBegin.DownloadBeginStart(builder)
    DownloadBegin.DownloadBeginAddAccount(builder, account)
    download_begin = DownloadBegin.DownloadBeginEnd(builder)
    builder.Finish(download_begin)
    source = builder.Output()
    # decode
    target = bytearray(len(source))
    target[:] = source
    xxx = DownloadBegin.DownloadBegin().GetRootAsDownloadBegin(target, 0)
    assert xxx.Account() == bytearray('test', 'utf-8')
