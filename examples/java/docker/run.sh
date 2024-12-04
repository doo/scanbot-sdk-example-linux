#!/bin/bash

CANONICAL=$(cd -P -- "$(dirname -- "$0")" && printf '%s\n' "$(pwd -P)")
DIR_WITH_IMAGES="<DIR_WITH_IMAGES>"

docker build --tag 'scanbot' ${CANONICAL}
docker run -m 2048m --memory-swap 2048m -v "${DIR_WITH_IMAGES}:/data" -v "${CANONICAL}/../:/example" -it 'scanbot' /example/docker/docker_flow.sh "/data"