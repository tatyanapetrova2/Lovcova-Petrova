using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

namespace Lovtsova_2
{
    [Serializable]
    public struct Set
    {
        public List<int> values;

        public Set(List<int> t)
        {
            this.values = t;
        }
        
    }

    
    class Program
    {
        public static void PrintSets(List<Set> t)
        {
            Console.WriteLine("Текущее состояние наборов: ");
            for(int j=0; j<t.Count; j++)
            {
                Console.WriteLine("Длина: " + t[j].values.Count.ToString());
                for (int i = 0; i < t[j].values.Count; i++)
                {
                    Console.Write(t[j].values[i].ToString() + '\t');
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }
        public static void AddElem(Set set, int value)
        {
            set.values.Add(value);
        }

        public static void DeleteElem(Set set, int value)
        {
            while (set.values.Remove(value));
        }

        static void Main(string[] args)
        {
            string answer;
            string path = "myxml.xml";
            List<Set> sets = new List<Set> { new Set(new List<int>()), new Set(new List<int>()) };
            XmlSerializer serializer = new XmlSerializer(typeof(List<Set>));

            Console.Write("Хотите восстановить состояние по xml (Y/N)? ");
            answer = Console.ReadLine();
            switch (answer)
            {
                case "Y":
                    try
                    {
                        using (FileStream fs = new FileStream(path, FileMode.Open))
                            sets = (List<Set>)serializer.Deserialize(fs);
                        Console.WriteLine("Состояние было восстановлено");
                    }
                    catch (FileNotFoundException e)
                    {
                        Console.WriteLine("Файл сохранений не найден. Состояние не было восстановлено");
                    }
                    break;
                case "N":
                    Console.WriteLine("Состояние не было восстановлено");
                    break;
                default:
                    using (FileStream fs = new FileStream(path, FileMode.Open))
                        sets = (List<Set>)serializer.Deserialize(fs);
                    Console.WriteLine("Некорректный ответ. На всякий случай, состояние было восстановлено");
                    break;
            }

            while (true)
            {
                PrintSets(sets);
                Console.WriteLine();
                Console.Write("Введите комманду (help - справка): ");
                answer = Console.ReadLine();
                //Console.Clear();

                switch (answer)
                {
                    case "help":
                        Console.WriteLine("Вам предоставлены команды: ");
                        Console.WriteLine("\thelp - показать эту прекрасную справку");
                        Console.WriteLine("\texit - выйти из программы");
                        Console.WriteLine("\tadd - добавить элемент в набор (интерфейс добавления будет предоставлен)");
                        Console.WriteLine("\tdelete - удалить элемент из набора (интерфейс удаления будет предоставлен)");
                        Console.WriteLine();
                        break;
                    case "exit":
                        Console.WriteLine("Счастья здоровья Вам! Для продолжения нажмите любую клавишу");
                        break;
                    case "add":
                        try
                        {
                            Console.Write("Введите номер набора: ");
                            int number = Int32.Parse(Console.ReadLine());
                            if (number != 1 && number != 2) goto default;
                            Console.Write("Введите число, которое хотите добавить: ");
                            int value = Int32.Parse(Console.ReadLine());
                            AddElem(sets[number - 1], value);
                           
                        }
                        catch (FormatException e)
                        {
                            Console.WriteLine("Отнюдь, и номер и добавляемое число должны быть целыми");
                        }
                        break;
                    case "delete":
                        try
                        {
                            Console.Write("Введите номер набора: ");
                            int number = Int32.Parse(Console.ReadLine());
                            if (number != 1 && number != 2) goto default;
                            Console.Write("Введите число, которое хотите удалить: ");
                            int value = Int32.Parse(Console.ReadLine());
                            DeleteElem(sets[number - 1], value);
                            
                        }
                        catch (FormatException e)
                        {
                            Console.WriteLine("Отнюдь, и номер и удаляемое число должны быть целыми");
                        }
                        break;
                    default:
                     
                        Console.WriteLine("Возможно, некорректный ввод, а возможно баг..");
                        break;

                }
                using (StreamWriter sw = new StreamWriter(path))
                {
                    serializer.Serialize(sw, sets);
                }
                if (answer == "exit") break;
            }
            Console.ReadLine();
        }
    }
}
