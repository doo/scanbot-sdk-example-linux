#!/bin/bash
DIR_WITH_IMAGES=$1

cd /example
./gradlew jar --no-daemon
java -jar build/libs/ScanbotSDKDocumentQualityStressTest-1.0.jar $DIR_WITH_IMAGES