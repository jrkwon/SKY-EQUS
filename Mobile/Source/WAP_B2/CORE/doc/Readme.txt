History for Generic WAP Client implemented according to WAP 1.1
===============================================================

Version notes:
--------------

Next

    Changes:
    
    The attributes "width" and "hight" in the "img" element were treated
    as an 8 bit integer in GWC this resulted in erroneous values if they
    exceeded 255. This problem no longer exist.

2.5.0.1

    Changes:
    
    The change for do-elements in 2.5 introduced a memory leak that is
    fixed.
    
    Known errors:
    
    None.
    
    Added files:

    None.
    
    Deleted files:
    
    None.


2.5

    Changes:
    
    A new interface for client port management has been created. For CO,
    this assures that GWC is not trying to use a session that is no
    longer valid, e.g. due to a lost CSD connection. Another benefit
    with this new interface is that it allows GWC to hold requests until
    a network connection exists.
    
    As a consequence of an error in the timer implementation, the
    expiration of timers with very small expiration times were not
    caught. This is now fixed.
    
    If the Cache-Directives MaxAge and MustRevalidate exist simultaneous
    in one single response, GWC now handles these Cache-Directives
    correctly.
    
    Erlier, when do-elements with a (explicit or implicit) name
    attribute equal to "" (empty string) were present in a card, not all
    do-elements were shown as as expected. This has now been changed.
    
    In connectionless mode, when the configuration variable
    configTIMEOUT was changed when a request was sent but no answer
    received yet, the answer would be discarded when received. Now, the
    answer will be treated as valid.
    
    Known errors:
    
    None.
    
    Added files:

    None.
    
    Deleted files:
    
    None.

2.4

    Changes:
    
    WTP has been optimized to use less ROM.
    
    The kernel has been changed in order to use less static RAM.
    
    The timer implementation has been changed. Now, not more than one
    timer is active at a given time.
    
    Known errors:
    
    None.
    
    Added files:

    wtppckg.c, wtppckg.h, wtppckg.ifc, wtpsuppt.c and wtpsuppt.h
    
    Deleted files:
    
    wtpctype.c and wtpctype.h

2.3.1

    Changes:
    
    None.
    
    Known errors:
    
    None.
    
    Added files:

    None.
    
    Deleted files:
    
    None.

2.3

    Changes:
    
    The WTLS API has been changed, se the release notes for the User's
    Manual.
    
    Known errors:
    
    None.
    
    Added files:

    capicrpt.[c|h]
    WTLSpckg.[c|h|ifc]
    
    Deleted files:
    
    WTLS.[h|ifc]
    WTLS1.c


2.2
    Changes:
    
    New status codes has been added, se the release notes for the User's
    Manual.
    
    The parser has been updated in order to use less of the call stack.
    
    Known errors:
    
    None.
    
    Added files:
    
    None.
    
    Deleted files:
    
    user_cc.h


2.1.2

    Changes:
    
    User id and password are now coded correctly for ASCII above 127.
    
    Minor changes to the WML string unescaping handling have been done.
    
    Contenttype is now recognized and forwarded when using file://
    scheme.

    The WMLS library function String.find has been changed to manage to
    match a substring even if a false match (just the begining of the
    substring) is found first.
    
    Earlier, when in the WMLS library function String.subString the
    startIndex and length parameters were added to find the end of the
    substring, no check for overflow was done. This resulted in an
    unexpected behaviour. This has been fixed by checking for overflow
    when adding the parameters.
    
    In WMLS, an internal parameter indicating the number of arguments in
    the library function Lang.parseInt was missing. Even if no
    functional influence has been observed, this has been changed.
    
    At termination of the GWC, instead of waiting a fixed time,
    termination is finished when all modules are ready with their own
    terminations.
    
    cachePrepared is sent as an answer to prepareCache even if the cache
    has a size of 0.
    
    In WSP, minor changes to the long integer handling have been
    done.
    
    In WTLS, the handling of the closePort signal has been changed.
    Earlier, the GWC could hang.
    
    A new configuration variable has been introduced to indicate whether
    the time returned by CLNTa_currentTime is GMT or not.
    
    WTLS now uses the same values for retransmission intervals and
    number of retransmissions as WTP.
    
    For connection-oriented mode, missing parameter values indicating
    the bearer and SMS-C resulted in requests being sent on UDP even
    though they were supposed to be sent on SMS bearer. This has been
    fixed.
    
    A parameter with a value being too high resulted in the maximum
    segment size for SMS to be set to 141 instead of the correct value
    140. This has now been fixed.
    
    In WDP, a check is added towards the maximum allowed buffer length
    before a buffer is allocated.
    
    For an invalid port, the received UDP-buffer was not released. This
    has been fixed now.
    
    The parameters that are pointers/arrays as well as an accompanying
    lengths are now checked for NULL in the connectors.
    
    Known errors:
    
    No known errors.
    
    Added or deleted files:
    
    No added or deleted files.


2.1.1

    Changes:

    An error in WTP resulting in freeing of memory that has already been
    deallocated. The error only appeared when GWC was initiated for a
    second time. This error has been fixed.

    WTP has been changed to wait with forwarding of the ClosePort signal
    until no transactions remain on the port about to be closed. Before
    the change, there were problems changing between connection-oriented
    and connection-less mode in a running GWC.

    Earlier, if WTP as an initiator was in state WAIT TIMEOUT and got a
    result PDU with the re-transmission indicator set, WTP replied with
    an acknowledgement PDU where the client and server addresses were
    switched. This resulted in unnecessary re-transmissions from the GW.
    This has been fixed.

    When the Gateway sends a WSP-Disconnect with status-code DISCONNECT,
    GWC now tries to make a new WSP-Connect with the same parameters as
    before. At the same time all ongoing requests (if there is anbody)
    are aborted and an error is sent to the device. Before, GWC did
    nothing when WSP-Disconnect with status-code DISCONNECT was received.
    
    Known errors:
    
    No known errors.
    
    Added or deleted files:
    
    No added or deleted files.


2.0.3

    First version based on the new component structure.
