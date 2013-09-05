INCLUDEPATH += $$PWD
PRECOMPILED_HEADER = $$PWD/global.h

SOURCES  += $$PWD/main.cpp \
            $$PWD/IMainWindow.cpp \
            $$PWD/CacheDb.cpp \
            $$PWD/CacheDb_p.cpp \
            $$PWD/Lib.cpp \
            $$PWD/ContactsModel.cpp \
            $$PWD/ContactDetailsModel.cpp \
            $$PWD/InboxModel.cpp \
            $$PWD/LibContacts.cpp \
            $$PWD/LibInbox.cpp \
            $$PWD/LibGvPhones.cpp \
            $$PWD/IPhoneAccount.cpp \
            $$PWD/IPhoneAccountFactory.cpp \
            $$PWD/IObserverFactory.cpp

HEADERS  += $$PWD/global.h \
            $$PWD/IOsDependent.h \
            $$PWD/IMainWindow.h \
            $$PWD/CacheDb.h \
            $$PWD/CacheDb_p.h \
            $$PWD/Lib.h \
            $$PWD/ContactsModel.h \
            $$PWD/ContactDetailsModel.h \
            $$PWD/InboxModel.h \
            $$PWD/LibContacts.h \
            $$PWD/LibInbox.h \
            $$PWD/LibGvPhones.h \
            $$PWD/IPhoneAccount.h \
            $$PWD/IPhoneAccountFactory.h \
            $$PWD/IObserverFactory.h \
            $$PWD/IObserver.h