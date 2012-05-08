﻿using WowPacketParser.Misc;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;

namespace WowPacketParser.Tests.Misc
{
    [TestClass]
    public class BiDictionaryTest
    {
        private BiDictionary<int, double> _biDictionary;

        [TestInitialize]
        public void Initialize()
        {
            _biDictionary = new BiDictionary<int, double>
            {
                {10, 5.0},
                {int.MaxValue, double.MinValue},
                {int.MinValue, double.MaxValue}
            };
        }

        [TestCleanup]
        public void Cleanup()
        {
            _biDictionary = null;
        }

        [TestMethod]
        public void TestCount()
        {
            Assert.AreEqual(3, _biDictionary.Count);
        }

        [TestMethod]
        public void TestClear()
        {
            _biDictionary.Clear();

            Assert.AreEqual(0, _biDictionary.Count);
        }

        [TestMethod]
        public void TestIndexers()
        {
            Assert.AreEqual(5.0, _biDictionary[10]);
            Assert.AreEqual(int.MaxValue, _biDictionary[double.MinValue]);

            Assert.AreEqual(default(int), _biDictionary[1.0]);
            Assert.AreEqual(default(double), _biDictionary[1]);
        }

        [TestMethod]
        public void TestGetters()
        {
            Assert.AreEqual(5.0, _biDictionary.GetByFirst(10));
            Assert.AreEqual(int.MaxValue, _biDictionary.GetBySecond(double.MinValue));

            Assert.AreEqual(default(double), _biDictionary.GetByFirst(1));
            Assert.AreEqual(default(int), _biDictionary.GetBySecond(1.0));
        }

        [TestMethod]
        public void TestTryGetters()
        {
            int first;
            double second;

            Assert.IsTrue(_biDictionary.TryGetByFirst(10, out second));
            Assert.IsTrue(_biDictionary.TryGetBySecond(double.MinValue, out first));

            Assert.AreEqual(5.0, second);
            Assert.AreEqual(int.MaxValue, first);

            Assert.IsFalse(_biDictionary.TryGetByFirst(1, out second));
            Assert.IsFalse(_biDictionary.TryGetBySecond(1.0, out first));
        }

        [TestMethod]
        public void TestEnumerator()
        {
            Assert.IsNotNull(_biDictionary.GetEnumerator());

            foreach (KeyValuePair<int, double> keyValuePair in _biDictionary)
                Assert.IsNotNull(keyValuePair);
        }

        [TestMethod]
        public void TestAdd()
        {
            _biDictionary.Add(default(int), default(double));

            Assert.AreEqual(4, _biDictionary.Count);
            Assert.AreEqual(default(double), _biDictionary[default(int)]);
        }
    }
}
