npm run build 
rsync -r *build icst@raspberrypi.local:~/pgit/finware-project/configuration-portal/unified-portal/

ssh icst@raspberrypi.local 'source ~/.zshrc && npx tsc && pm2 restart 0'
