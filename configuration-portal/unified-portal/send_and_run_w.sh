npm run build
scp -o NoHostAuthenticationForLocalhost=yes -r build src-backend icst@raspberrypi.local:~/iotproj/finware-project/configuration-portal/unified-portal

ssh -o NoHostAuthenticationForLocalhost=yes icst@raspberrypi.local 'source ~/.bashrc && cd ~/iotproj/finware-project/configuration-portal/unified-portal && /home/icst/.nvm/versions/node/v18.7.0/bin/npx tsc'

echo "FINISHED"
