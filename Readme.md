Thread pool
======================

A minimal thread pool for runnnig bash commands in parallell.


Usage example
-----------------

### Running simple command (meaningless)

```bash
echo "echo hej" | ./thread_pool
```

### Running two commands (almost meaningless)
```bash
echo -e "echo hej\n echo san" | ./thread_pool
```


### Running with output from other application
```bash
./other_app | ./thread_pool
```



