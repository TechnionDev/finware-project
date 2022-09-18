npm run build
scp -o NoHostAuthenticationForLocalhost=yes -r build src-backend icst@raspberrypi.local:~/opt/finware-project/configuration-portal/

ssh -o NoHostAuthenticationForLocalhost=yes icst@raspberrypi.local 'source ~/.bashrc && cd /opt/finware-project/configuration-portal && /home/icst/.nvm/versions/node/v18.7.0/bin/npx tsc'

echo "FINISHED"
