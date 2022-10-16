npm run build 
rsync -r build icst@finware.local:/opt/finware-project/configuration-portal/
rsync -r src-backend icst@finware.local:/opt/finware-project/configuration-portal/

ssh icst@finware.local 'source ~/.bashrc && cd /opt/finware-project/configuration-portal/ && pm2 stop 0 && /home/icst/.nvm/versions/node/v18.7.0/bin/npx tsc && pm2 start 0'
