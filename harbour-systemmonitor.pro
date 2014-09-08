
# The name of your application
TARGET = harbour-systemmonitor
TEMPLATE = subdirs

SUBDIRS += app
SUBDIRS += daemon

OTHER_FILES += \
    rpm/harbour-systemmonitor.yaml \
    rpm/harbour-systemmonitor.changes \
    rpm/harbour-systemmonitor.spec
