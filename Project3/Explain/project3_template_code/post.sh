#此檔案可以用於 將 rule 的 json 檔 post 至 onos, 同學可以將註解去掉並自行更改
# post switch 1 的 json 檔
nfiles=switch1_*.json
for file in $nfiles
do
    echo "\n----$file----\n"
    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000001'
done

# post switch 2 的 json 檔
#nfiles=switch2_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000002'
#done

# post switch 3 的 json 檔
#nfiles=switch3_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000003'
#done

# post switch 4 的 json 檔
#nfiles=switch4_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000004'
#done

# post switch 5 的 json 檔
#nfiles=switch5_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000005'
#done

# post switch 6 的 json 檔
#nfiles=switch6_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000006'
#done

# post task_3 的 json 檔
#nfiles=load_balance_*.json
#for file in $nfiles
#do
#    echo "\n----$file----\n"
#    curl -u onos:rocks -X POST -H 'Content-Type: application/json' -d"@$file" 'http://localhost:8181/onos/v1/flows/of:0000000000000001'
#done
