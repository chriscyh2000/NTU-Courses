# WP1101 HW9

請先執行以下指令：
```
cd frontend
yarn install
cd ../backend
yarn install
```
並且進到`backend/node_modules/uuid/package.json`，將`"exports"`的部分改成：
```
"exports": {
    "./v4": "./dist/v4.js",
    ".": {
      "node": {
        "module": "./dist/esm-node/index.js",
        "require": "./dist/index.js",
        "import": "./wrapper.mjs"
      },
      "default": "./dist/esm-browser/index.js"
    },
    "./package.json": "./package.json"
  },
```