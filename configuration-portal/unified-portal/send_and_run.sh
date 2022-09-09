npm run build && npx tsc
rsync -r *build icst@raspberrypi.local:~/pgit/finware-project/configuration-portal/unified-portal/

ssh icst@raspberrypi.local 'source ~/.zshrc && pm2 restart 0'
