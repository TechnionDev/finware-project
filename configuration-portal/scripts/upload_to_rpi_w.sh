npm run build
scp -o NoHostAuthenticationForLocalhost=yes -r build src-backend package.json icst@finware.local:/opt/finware-project/configuration-portal/

ssh -o NoHostAuthenticationForLocalhost=yes icst@finware.local 'source ~/.bashrc && cd /opt/finware-project/configuration-portal && /home/icst/.nvm/versions/node/v18.7.0/bin/npx tsc && /home/icst/.nvm/versions/node/v18.7.0/bin/pm2 restart finware'

echo "FINISHED"
