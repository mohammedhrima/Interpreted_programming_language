
def func(n):
    if n > 3:
        return n + 1
    else:
        return func(n - 1)

m = func(6)
print(m)
