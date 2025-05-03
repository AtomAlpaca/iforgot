# iforgot
An AI-powered command line tool to rectify error linux commands bases on command history

## Get started
```bash
cd src
g++ main.cpp -o iforgot -Iollama-hpp/singleheader -std=c++20 -O2
```

## Help
```bash
iforgot --help -h: input help info
iforgot --init -i [model name]: init model base on [model name](qwen:0.6b in default)
iforgot <command>
```
