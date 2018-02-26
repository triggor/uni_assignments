using System;
using System.IO;
using Npgsql;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Text.RegularExpressions;
using Newtonsoft.Json.Linq;
using System.Linq;
using System.Data;



namespace Project
{
    class Program
    {
        static void Main(string[] args)
        {
            //filename with input data, located in the project directory:
            string inputPath = "input";
            try
            {
                if (!File.Exists(inputPath))
                {
                    Console.Error.WriteLine("ERROR: INPUT FILE MISSING!\n"
                        + "Make sure the file with input data has correct name and is located in the project directory.");
                }
            }
            catch (Exception){};

            try
            {
                //open the input file:
                using (StreamReader sr = new StreamReader(new FileStream(inputPath, FileMode.Open)))
                {
                    DatabaseQueries c = new DatabaseQueries();

                    //for each line in the input file:
                    while (sr.Peek() >= 0)
                    {
                        //read non-empty line from input file and parse it to a dictionary:
                        string line = sr.ReadLine();
                        string temp = Regex.Replace(line, @"\s+", string.Empty);
                        if(String.Compare(temp, string.Empty) == 0) continue;
                        var parsedDict = JsonConvert.DeserializeObject<Dictionary<String,Dictionary<String,String>>>(line);
                        
                        //get method name:
                        List<string> keyList = new List<string>(parsedDict.Keys);
                        string methodName = keyList[0];
                        //Console.WriteLine("METHOD: " + methodName);

                        //get method arguments:
                        List<string> keyListArg = new List<string>(parsedDict[methodName].Keys);
                        int paramLength = keyListArg.Count;
                        Object[] methodParams = new Object[paramLength];
                        for(int i=0; i<paramLength; i++)
                        {
                            methodParams[i] = parsedDict[methodName][keyListArg[i]];
                            //Console.Write("\t"+keyListArg[i]+":");
                            //Console.WriteLine(methodParams[i]);
                        }
                        
                        //invoke the method using reflection and bring out the returned JSON:
                        Type type = typeof(DatabaseQueries);
                        MethodInfo method = type.GetMethod(methodName.First().ToString().ToUpper() + methodName.Substring(1));
                        string result = (string)method.Invoke(c, methodParams);
                        Console.WriteLine(result);
                    }
                }
            }
            catch(Exception e)
            {
                Console.Error.WriteLine("ERROR OCCURED! " + e.Data + ": " + e.Message + "\n" + e.StackTrace);
            }
        }
    }

//*********************************************************************************************************************************//
//*********************************************************************************************************************************//
//*********************************************************************************************************************************//

    public class DatabaseQueries
    {
        const string okStat = "OK";
        const string niStat = "NOT IMPLEMENTED";
        const string erStat = "ERROR";
        private NpgsqlConnection conn = new NpgsqlConnection();

        private bool ValidCredentials(string login, string password, string role)
        {
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT COUNT(*) FROM users WHERE login=@login AND password=@password AND role=@role::rolelist";
                    query.Parameters.AddWithValue("login", login);
                    query.Parameters.AddWithValue("password", password);
                    query.Parameters.AddWithValue("role", role);
                    using (var reader = query.ExecuteReader()) {
                        while(reader.Read()){
                             if(reader.GetInt32(0) != 1) 
                             {
                                 //Console.Error.WriteLine("ERROR at CHECKCREDENTIALS!");
                                 return false;
                             }
                        }
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at CHECKCREDENTIALS! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return false;
            }
            return true;
        }

        /*============================== Nawiązywanie połączenia i definiowanie danych organizatora ==============================*/

        // (*) open <baza> <login> <password>
        // przekazuje dane umożliwiające podłączenie Twojego programu do bazy - nazwę bazy, login oraz hasło, 
        // wywoływane dokładnie jeden raz, w pierwszej linii wejścia
        // zwraca status OK/ERROR w zależności od tego czy udało się nawiązać połączenie z bazą 
        public string Open(string baza, string login, string password)
        {            

            try
            {
                conn.ConnectionString = "Host=localhost; Username=" + login + ";Password=" + password + ";Database=" + baza;
                conn.Open();
                //Console.WriteLine(conn.State);
            }
            catch (Exception)// e)

            {
                //Console.Error.WriteLine("ERROR! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }

//------------------------------------READING DATA FROM PHYSICAL MODEL AND BUILDING THE DATABASE-------------------------//

            // const string DBPath = "db_project_physical.sql";
            // try
            // {
            //     if (!File.Exists(DBPath))
            //     {
            //         Console.Error.WriteLine("ERROR: DATABASE FILE MISSING!\n"
            //             + "Make sure the file with database has correct name and is located in the project directory.");
            //             return JsonConvert.SerializeObject(new {status = erStat});
            //     }
            // }
            // catch (Exception)
            // {
            //     return JsonConvert.SerializeObject(new {status = erStat});
            // };

            // try
            // {
            //     //open the database file:
            //     using (StreamReader sr = new StreamReader(new FileStream(DBPath, FileMode.Open)))
            //     {
            //         //DatabaseQueries c = new DatabaseQueries();
            //         using (var inserter = new NpgsqlCommand())
            //         {
            //             inserter.Connection = conn;
            //             //for each line in the database file:
            //             while (sr.Peek() >= 0)
            //             {
            //                 //read non-empty line from database file and write it to STDOUT:
            //                 inserter.CommandText += sr.ReadLine();
            //             }
            //             inserter.ExecuteNonQuery();
                        
            //         }
            //     }
            // }
            // catch (Exception e)
            // {
            //     Console.Error.WriteLine("ERROR at READING DATA! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            //     return JsonConvert.SerializeObject(new {status = erStat});
            // };
//------------------------------------------------------------------------------------------------------------------------//

            if(conn.State.ToString() != "Open") return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*) organizer <secret> <newlogin> <newpassword> 
        // tworzy uczestnika <newlogin> z uprawnieniami organizatora i hasłem <newpassword>, argument <secret> musi być 
        // równy d8578edf8458ce06fbc5bb76a58c5ca4 
        // zwraca status OK/ERROR 
        public string Organizer(string secret, string newlogin, string newpassword)
        {
            const string defaultSecret = "d8578edf8458ce06fbc5bb76a58c5ca4";
            if(secret != defaultSecret) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO users VALUES (@login, @password, 'organizer')";
                    inserter.Parameters.AddWithValue("login", newlogin);
                    inserter.Parameters.AddWithValue("password", newpassword);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at ORGANIZER! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        /*==================================================== Operacje modyfikujące bazę =========================================*/

        // (*O) event <login> <password> <eventname> <start_timestamp> <end_timestamp> 
        // rejestracja wydarzenia, napis <eventname> jest unikalny
        public string Event(string login, string password, string eventname, string start_timestamp, string end_timestamp)
        {
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO events VALUES (@eventname, @login, @start_timestamp::date, " +
                                                                                        "@end_timestamp::date)";
                    inserter.Parameters.AddWithValue("eventname", eventname);
                    inserter.Parameters.AddWithValue("login", login);
                    inserter.Parameters.AddWithValue("start_timestamp", start_timestamp);
                    inserter.Parameters.AddWithValue("end_timestamp", end_timestamp);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at EVENT! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*O) user <login> <password> <newlogin> <newpassword> 
        // rejestracja nowego uczestnika <login> i <password> służą do autoryzacji wywołującego funkcję, 
        // który musi posiadać uprawnienia organizatora, <newlogin> <newpassword> są danymi nowego uczestnika, <newlogin> jest unikalny
        public string User(string login, string password, string newlogin, string newpassword)
        {
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO users VALUES (@login, @password, 'participant')";
                    inserter.Parameters.AddWithValue("login", newlogin);
                    inserter.Parameters.AddWithValue("password", newpassword);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at USER! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*O) talk <login> <password> <speakerlogin> <talk> <title> <start_timestamp> <room> <initial_evaluation> <eventname> 
        // rejestracja referatu/zatwierdzenie referatu spontanicznego, <talk> jest unikalnym identyfikatorem referatu, 
        // <initial_evaluation> jest oceną organizatora w skali 0-10 – jest to ocena traktowana tak samo jak ocena uczestnika 
        // obecnego na referacie, <eventname> jest nazwą wydarzenia, którego częścią jest dany referat - może być pustym napisem, 
        // co oznacza, że referat nie jest przydzielony do jakiegokolwiek wydarzenia
        public string Talk(string login, string password, string speakerlogin, string talk, string title, string start_timestamp, 
                                                                            string room, string initial_evaluation, string eventname)
        {
            //TODO: rewrite to SQL function
            int proposedTalk = -1;
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});

            //checking if the talk is new or proposed:
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;

                    //check if the requested talk is modifiable:
                    query.CommandText = "SELECT COUNT(*) FROM talks WHERE talk_id=@talk AND NOT status='proposed'";
                    query.Parameters.AddWithValue("talk", talk);
                    using (var reader = query.ExecuteReader()) {
                        while(reader.Read()){
                             if(reader.GetInt32(0) == 1) 
                             {
                                //Console.Error.WriteLine("TALK IS NOT MODIFIABLE!");
                                return JsonConvert.SerializeObject(new {status = erStat});
                             }
                        }
                    }

                    //check if the requested talk is proposed:
                    query.CommandText = "SELECT COUNT(*) FROM talks WHERE talk_id=@talk AND status='proposed'";
                    query.Parameters.AddWithValue("talk", talk);
                    using (var reader = query.ExecuteReader()) {
                        while(reader.Read()){
                             if(reader.GetInt32(0) == 1) 
                             {
                                proposedTalk = 1;
                                //Console.WriteLine("TALK_PROPOSED_CHECK: YES");
                                
                             }
                             else
                             {
                                proposedTalk = 0;
                                //Console.WriteLine("TALK_PROPOSED_CHECK: NOT");
                             }
                        }
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at TALK_PROPOSITION_CHECK! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }

            //confirming proposed talk:
            if(proposedTalk == 1)
            {
                //Console.Error.WriteLine("INSIDE OF PROPOSED TALK....");
                try
                {
                    using (var updater = new NpgsqlCommand())
                    {
                        updater.Connection = conn;
                        updater.CommandText = "UPDATE talks SET talk_id=@talk_id, event=@event, speaker=@speaker, " + 
                            "description=@description, start_time=@start_timestamp::timestamp without time zone, " +
                            "status=@status::statuslist, room=@room::smallint, coordinator=@coordinator, " +
                            "initial_eval=@initial_eval::smallint WHERE talk_id=@talk_id AND status='proposed'";
                        if(String.Compare(eventname, string.Empty) == 0) 
                            updater.Parameters.AddWithValue("event", DBNull.Value);
                        else 
                            updater.Parameters.AddWithValue("event", eventname);

                        updater.Parameters.AddWithValue("talk_id", talk);
                        updater.Parameters.AddWithValue("speaker", speakerlogin);
                        updater.Parameters.AddWithValue("description", title);
                        updater.Parameters.AddWithValue("start_timestamp", start_timestamp);
                        updater.Parameters.AddWithValue("status", "confirmed");
                        updater.Parameters.AddWithValue("room", room);
                        updater.Parameters.AddWithValue("coordinator", login);
                        updater.Parameters.AddWithValue("initial_eval", initial_evaluation);
                        updater.ExecuteNonQuery();
                    }
                }
                catch (Exception)// e)
                {
                    //Console.Error.WriteLine("ERROR at TALK! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                    return JsonConvert.SerializeObject(new {status = erStat});
                }
            }

            //inserting a new talk:
            else if(proposedTalk == 0)
            {
                try
                {
                    using (var inserter = new NpgsqlCommand())
                    {
                        inserter.Connection = conn;
                        inserter.CommandText = "INSERT INTO talks VALUES (@talk_id, @event, @speaker, @description, " +
                        "@start_timestamp::timestamp without time zone, @status::statuslist, @room::smallint, @coordinator, " +
                                                                                                "@initial_eval::smallint)";
                        
                        if(String.Compare(eventname, string.Empty) == 0) 
                            inserter.Parameters.AddWithValue("event", DBNull.Value);
                        else 
                            inserter.Parameters.AddWithValue("event", eventname);

                        inserter.Parameters.AddWithValue("talk_id", talk);
                        inserter.Parameters.AddWithValue("speaker", speakerlogin);
                        inserter.Parameters.AddWithValue("description", title);
                        inserter.Parameters.AddWithValue("start_timestamp", start_timestamp);
                        inserter.Parameters.AddWithValue("status", "planned");
                        inserter.Parameters.AddWithValue("room", room);
                        inserter.Parameters.AddWithValue("coordinator", login);
                        inserter.Parameters.AddWithValue("initial_eval", initial_evaluation);
                        inserter.ExecuteNonQuery();
                    }
                }
                catch (Exception)// e)
                {
                    //Console.Error.WriteLine("ERROR at TALK! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                    return JsonConvert.SerializeObject(new {status = erStat});
                }
            }

            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*U) register_user_for_event <login> <password> <eventname> 
        // rejestracja uczestnika <login> na wydarzenie <eventname>
        public string Register_user_for_event(string login, string password, string eventname)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO participant_registrations VALUES (@participant, @event)";
                    inserter.Parameters.AddWithValue("participant", login);
                    inserter.Parameters.AddWithValue("event", eventname);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at REGISTER_USER_FOR_EVENT! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*U) attendance <login> <password> <talk> 
        // odnotowanie faktycznej obecności uczestnika <login> na referacie <talk>
        public string Attendance(string login, string password, string talk)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO talk_attendance VALUES (@participant, @talk, 'present')";
                    inserter.Parameters.AddWithValue("participant", login);
                    inserter.Parameters.AddWithValue("talk", talk);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at ATTENDANCE! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (*U) evaluation <login> <password> <talk> <rating> 
        // ocena referatu <talk> w skali 0-10 przez uczestnika <login>
        public string Evaluation(string login, string password, string talk, string rating)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var inserter = new NpgsqlCommand())
                {
                    inserter.Connection = conn;
                    inserter.CommandText = "INSERT INTO talk_evaluation VALUES (@participant, @talk, @evaluation::smallint)";
                    inserter.Parameters.AddWithValue("participant", login);
                    inserter.Parameters.AddWithValue("talk", talk);
                    inserter.Parameters.AddWithValue("evaluation", rating);
                    inserter.ExecuteNonQuery();
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at EVALUATION! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
                return JsonConvert.SerializeObject(new {status = erStat});
            }
            return JsonConvert.SerializeObject(new {status = okStat});
        }

        // (O) reject <login> <password> <talk> 
        // usuwa referat spontaniczny <talk> z listy zaproponowanych,
        public string Reject(string login, string password, string talk)
        {
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U) proposal  <login> <password> <talk> <title> <start_timestamp> 
        // propozycja referatu spontanicznego, <talk> - unikalny identyfikator referatu
        public string Proposal(string login, string password, string talk, string title, string start_timestamp)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            // try
            // {
            //     using (var inserter = new NpgsqlCommand())
            //     {
            //         inserter.Connection = conn;
            //         inserter.CommandText = "INSERT INTO talks VALUES (@talk_id, @event, @speaker, @description, " +
            //         "@start_timestamp::timestamp without time zone, @status::statuslist, @room::smallint, @coordinator, " +
            //                                                                                 "@initial_eval::smallint)";
                    
            //         inserter.Parameters.AddWithValue("event", DBNull.Value);
            //         inserter.Parameters.AddWithValue("talk_id", talk);
            //         inserter.Parameters.AddWithValue("speaker", login);
            //         inserter.Parameters.AddWithValue("description", title);
            //         inserter.Parameters.AddWithValue("start_timestamp", start_timestamp);
            //         inserter.Parameters.AddWithValue("status", "proposed");
            //         inserter.Parameters.AddWithValue("room", DBNull.Value);
            //         inserter.Parameters.AddWithValue("coordinator", "organ1");
            //         inserter.Parameters.AddWithValue("initial_eval", DBNull.Value);
            //         inserter.ExecuteNonQuery();
            //     }
            // }
            // catch (Exception e)
            // {
            //     Console.Error.WriteLine("ERROR at PROPOSAL! " + e.Data + ": " + e.Message + "\n" + e.StackTrace);
            //     return JsonConvert.SerializeObject(new {status = erStat});
            // }
            

            // return JsonConvert.SerializeObject(new {status = okStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U) friends <login1> <password> <login2> 
        // uczestnik <login1> chce nawiązać znajomość z uczestnikiem <login2>, znajomość uznajemy za nawiązaną jeśli obaj 
        // uczestnicy chcą ją nawiązać tj. po wywołaniach friends <login1> <password1> <login2> i friends <login2> <password2> <login1>
        public string Friends(string login1, string password, string login2)
        {
            if(!ValidCredentials(login1, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        /*================================================ Pozostałe operacje ====================================================*/

        // (*N) user_plan <login> <limit> 
        // zwraca plan najbliższych referatów z wydarzeń, na które dany uczestnik jest zapisany (wg rejestracji na wydarzenia) 
        // posortowany wg czasu rozpoczęcia, wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
        // Atrybuty zwracanych krotek: 
        // <login> <talk> <start_timestamp> <title> <room>
        public string User_plan(string login, string limit)
        {
            const int MAX_INT = 2147483647;
            if(String.Compare(limit, "0") == 0) limit = MAX_INT.ToString(); 
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT * FROM user_plan(@user, @limit::int)";
                    query.Parameters.AddWithValue("user", login);
                    query.Parameters.AddWithValue("limit", limit);
                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                login = dr.GetString(0),
                                talk = dr.GetString(1),
                                start_timestamp = dr.GetDateTime(2).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(3),
                                room = dr.GetInt16(4).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at USER_PLAN! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (*N) day_plan <timestamp> 
        // zwraca listę wszystkich referatów zaplanowanych na dany dzień posortowaną rosnąco wg sal, w drugiej kolejności 
        // wg czasu rozpoczęcia
        //  <talk> <start_timestamp> <title> <room>
        public string Day_plan(string timestamp)
        {
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT * FROM day_plan(@timestamp::timestamp without time zone)";
                    query.Parameters.AddWithValue("timestamp", timestamp);
                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                talk = dr.GetString(0),
                                start_timestamp = dr.GetDateTime(1).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(2),
                                room = dr.GetInt16(3).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at DAY_PLAN! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (*N) best_talks <start_timestamp> <end_timestamp> <limit> <all> 
        // zwraca referaty rozpoczynające się w  danym przedziale czasowym posortowane malejąco wg średniej oceny uczestników, 
        // przy czym jeśli <all> jest równe 1 należy wziąć pod uwagę wszystkie oceny, w przeciwnym przypadku tylko oceny uczestników, 
        // którzy byli na referacie obecni, wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <start_timestamp> <title> <room>
        public string Best_talks(string start_timestamp, string end_timestamp, string limit, string all)
        {
            const int MAX_INT = 2147483647;
            if(String.Compare(limit, "0") == 0) limit = MAX_INT.ToString(); 
            int allnum = 0;
            Int32.TryParse(all, out allnum);
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    if(allnum == 1)
                    {
                        query.CommandText = "SELECT * FROM best_talks_all(@start_timestamp::timestamp without time zone, " +
                                            "@end_timestamp::timestamp without time zone, @limit::int, @all::int)";
                    }
                    else
                    {
                    query.CommandText = "SELECT * FROM best_talks(@start_timestamp::timestamp without time zone, " +
                                            "@end_timestamp::timestamp without time zone, @limit::int, @all::int)";
                    }
                    query.Parameters.AddWithValue("start_timestamp", start_timestamp);
                    query.Parameters.AddWithValue("end_timestamp", end_timestamp);
                    query.Parameters.AddWithValue("limit", limit);
                    query.Parameters.AddWithValue("all", all);

                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                talk = dr.GetString(0),
                                start_timestamp = dr.GetDateTime(1).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(2),
                                room = dr.GetInt16(3).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at BEST_TALKS! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (*N) most_popular_talks <start_timestamp> <end_timestamp> <limit> 
        // zwraca referaty rozpoczynające się w podanym przedziału czasowego posortowane malejąco wg obecności, wypisuje 
        // pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <start_timestamp> <title> <room>
        public string Most_popular_talks(string start_timestamp, string end_timestamp, string limit)
        {
            const int MAX_INT = 2147483647;
            if(String.Compare(limit, "0") == 0) limit = MAX_INT.ToString(); 
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT * FROM most_popular_talks(@start_timestamp::timestamp without time zone, " +
                                                            "@end_timestamp::timestamp without time zone, @limit::int)";
                    query.Parameters.AddWithValue("start_timestamp", start_timestamp);
                    query.Parameters.AddWithValue("end_timestamp", end_timestamp);
                    query.Parameters.AddWithValue("limit", limit);

                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                talk = dr.GetString(0),
                                start_timestamp = dr.GetDateTime(1).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(2),
                                room = dr.GetInt16(3).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at MOST_POPULAR_TALKS! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (*U) attended_talks <login> <password> 
        // zwraca dla danego uczestnika referaty, na których był obecny 
        //  <talk> <start_timestamp> <title> <room>
        public string Attended_talks(string login, string password)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT * FROM attended_talks(@login::varchar(20))";
                    query.Parameters.AddWithValue("login", login);

                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                talk = dr.GetString(0),
                                start_timestamp = dr.GetDateTime(1).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(2),
                                room = dr.GetInt16(3).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at ATTENDED_TALKS! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (*O) abandoned_talks <login> <password>  <limit> 
        // zwraca listę referatów posortowaną malejąco wg liczby uczestników <number> zarejestrowanych na wydarzenie obejmujące referat, 
        // którzy nie byli na tym referacie obecni, wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <start_timestamp> <title> <room> <number>
        public string Abandoned_talks(string login, string password, string limit)
        {
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});
            const int MAX_INT = 2147483647;
            if(String.Compare(limit, "0") == 0) limit = MAX_INT.ToString(); 
            try
            {
                using (var query = new NpgsqlCommand())
                {
                    query.Connection = conn;
                    query.CommandText = "SELECT * FROM abandoned_talks(@limit::int)";
                    query.Parameters.AddWithValue("limit", limit);

                    using (var dr = query.ExecuteReader()) {
                        List<Object> results = new List<Object>();
                        while(dr.Read()){
                            var obj = new 
                            {
                                talk = dr.GetString(0),
                                start_timestamp = dr.GetDateTime(1).ToString("yyyy-MM-dd HH:mm:ss"),
                                title = dr.GetString(2),
                                room = dr.GetInt16(3).ToString(),
                                number = dr.GetInt16(4).ToString(),
                            };
                            results.Add(obj);                        
                        }
                        return JsonConvert.SerializeObject(new {status=okStat, data=results});
                    }
                }
            }
            catch (Exception)// e)
            {
                //Console.Error.WriteLine("ERROR at ABANDONED_TALKS! " + e.Data + ": " + e.Message);// + "\n" + e.StackTrace);
            }
            return JsonConvert.SerializeObject(new {status = erStat});
        }

        // (N) recently_added_talks <limit> 
        // zwraca listę ostatnio zarejestrowanych referatów, wypisuje ostatnie <limit> referatów wg daty zarejestrowania, 
        // przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <speakerlogin> <start_timestamp> <title> <room>
        public string Recently_added_talks(string limit)
        {
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U/O) rejected_talks <login> <password> 
        // jeśli wywołujący ma uprawnienia organizatora zwraca listę wszystkich odrzuconych referatów spontanicznych, w przeciwnym 
        // przypadku listę odrzuconych referatów wywołującego ją uczestnika 
        //  <talk> <speakerlogin> <start_timestamp> <title>
        public string Rejected_talks(string login, string password)
        {
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (O) proposals <login> <password> 
        // zwraca listę propozycji referatów spontanicznych do zatwierdzenia lub odrzucenia, zatwierdzenie lub odrzucenie referatu 
        // polega na wywołaniu przez organizatora funkcji talk lub reject z odpowiednimi parametrami
        //  <talk> <speakerlogin> <start_timestamp> <title>
        public string Proposals(string login, string password)
        {
            if(!ValidCredentials(login, password, "organizer")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U) friends_talks <login> <password> <start_timestamp> <end_timestamp> <limit> // lista referatów  rozpoczynających się 
        // w podanym przedziale czasowym wygłaszanych przez znajomych danego uczestnika posortowana wg czasu rozpoczęcia, 
        // wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <speakerlogin> <start_timestamp> <title> <room>
        public string Friends_talks(string login, string password, string start_timestamp, string end_timestamp, string limit)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U) friends_events <login> <password> <eventname> 
        // lista znajomych uczestniczących w danym wydarzeniu
        //  <login> <eventname> <friendlogin> 
        public string Friends_events(string login, string password, string eventname)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        // (U) recommended_talks <login> <password> <start_timestamp> <end_timestamp> <limit> 
        // zwraca referaty rozpoczynające się w podanym przedziale czasowym, które mogą zainteresować danego uczestnika 
        // (zaproponuj parametr <score> obliczany na podstawie dostępnych danych – ocen, obecności, znajomości itp.), 
        // wypisuje pierwsze <limit> referatów wg nalepszego <score>, przy czym 0 oznacza, że należy wypisać wszystkie
        //  <talk> <speakerlogin> <start_timestamp> <title> <room> <score>
        public string Recommended_talks(string login, string password, string start_timestamp, string end_timestamp, string limit)
        {
            if(!ValidCredentials(login, password, "participant")) return JsonConvert.SerializeObject(new {status = erStat});
            return JsonConvert.SerializeObject(new {status = niStat});
        }

        /*=======================================================================================================================*/
        
    }
}
