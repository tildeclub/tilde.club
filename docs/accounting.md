# Accounting

See who has been logged in the longest:

```
for a in `users`; do echo `ac $a` $a; done|sort|uniq|cut -d' ' -f2,3|sort -n
```