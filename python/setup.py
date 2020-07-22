# Copyright (c) 2017-2020, Hans Erik Thrane

""" setup.py """

import os

from setuptools import setup, find_packages

setup(
    name='roq',
    version=os.environ.get('GIT_DESCRIBE_TAG', '0.0.0'),
    license='MIT',
    author='Roq Trading Solutions',
    author_email='info@roq-trading.com',
    url='https://roq-trading.com/docs/',
    long_description=('Roq API.'),
    package_dir={'':'src'},
    packages=find_packages('src', exclude=['tests']),
    include_package_data=False,
    requires=[
        'flatbuffers',
    ],
    setup_requires=[
        'pytest-runner',
    ],
    tests_require=[
        'pytest',
    ],
    test_suite='tests',
    description='Python API to communicate with market gateways',
    classifiers=[
    ],
    project_urls={
        'Documentation': 'https://roq-trading.com/docs/',
        'Source': 'https://github.com/roq-trading/roq-api',
    },
)
