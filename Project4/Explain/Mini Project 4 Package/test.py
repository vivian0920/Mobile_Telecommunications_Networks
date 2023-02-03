import datetime
def run(*args):
    count=args[0].count('/')
    if count==2 and len(args[0])==10:
        today = datetime.date.today()
        first=args[0].index('/')
        second=args[0].index('/',4,9)
        arr = args[0].split('/')
        target= datetime.date(int(arr[0]), int(arr[1]), int(arr[2]))
        diff=(target-today).days  
        if diff>=0:
            return "還剩下"+str(diff)+"天"
        else:
            diff=abs(target-today).days 
            return "已過去"+str(diff)+"天"
    else:
        return "不好意思請輸入正確的格式呦！ 如：2023/01/01"



