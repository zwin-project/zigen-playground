module.exports = {
  env: {
    browser: true,
    es6: true,
    node: true,
  },
  /* we will create a react app in the future. */
  plugins: ['react', '@typescript-eslint', 'prettier'],
  extends: ['plugin:react/recommended', 'standard', 'prettier'],
  globals: {
    Atomics: 'readonly',
    SharedArrayBuffer: 'readonly',
  },
  parser: '@typescript-eslint/parser',
  parserOptions: {
    ecmaFeatures: {
      jsx: true,
    },
    ecmaVersion: 11,
    sourceType: 'module',
  },
  overrides: [
    {
      files: ['./**/*.ts', './**/*.tsx'],
      extends: ['plugin:@typescript-eslint/recommended'],
      rules: {
        '@typescript-eslint/explicit-function-return-type': ['off'],
        '@typescript-eslint/no-use-before-define': ['off'],
        '@typescript-eslint/member-delimiter-style': ['off'],
        'no-useless-constructor': ['off'],
      },
    },
  ],
  rules: {
    'react/prop-types': ['off'],
    'prettier/prettier': ['error'],
    'no-new': ['off'],
    'no-unused-expressions': ['off'],
  },
  settings: {
    react: {
      version: 'detect',
    },
  },
}
