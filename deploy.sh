#!/usr/bin/env sh

SSH_CMD='ssh -q lvuser:@roboRIO-2358-FRC.local'

echo "disposing of old code..."
$SSH_CMD 'rm FRCUserProgram'

echo "uploading new code..."
scp -q out/Default/FRCUserProgram lvuser@roboRIO-2358-FRC.local:FRCUserProgram

echo "murdering netconsole..."
$SSH_CMD 'killall -q netconsole-host'

echo "sourcing environment..."
$SSH_CMD '. /etc/profile.d/natinst-path.sh'

echo "changing code permissions..."
$SSH_CMD 'chmod a+x FRCUserProgram'

echo "fragging old code..."
$SSH_CMD 'killall -q FRCUserProgram'

echo "syncing..."
$SSH_CMD 'sync'