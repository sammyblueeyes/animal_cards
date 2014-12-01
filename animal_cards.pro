HEADERS       = Animal_card_app.h
SOURCES       = Animal_card_app.cpp \
                main.cpp

# install
target.path = .
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS Animal_card_app.pro
sources.path = .
INSTALLS += target sources

maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

