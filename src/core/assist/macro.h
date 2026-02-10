/**
 * Copyright 2025 Viper authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _VIPER_CORE_ASSIST_MACRO_H_
#define _VIPER_CORE_ASSIST_MACRO_H_

#define ANONYMOUS_VAR_NAME(t1, t2) t1##t2
#define ANONYMOUS_VAR(t1, t2) ANONYMOUS_VAR_NAME(t1, t2)

#define BEGIN_TRY \
    try           \
    {

#define END_TRY }

#define BEGIN_CATCH(exception, ex) \
    catch (const exception& ex)    \
    {

#define END_CATCH }

#define END_TRY_BEGIN_CATCH(exception, ex) \
    }                                      \
    catch (const exception& ex)            \
    {

#define IGNORE_CATCH_ALL \
    catch (...)          \
    {                    \
    }

#define END_CATCH_AND_BEGIN_CATCH_ALL \
    }                                 \
    catch (...)                       \
    {

#define END_TRY_AND_BEGIN_CATCH_ALL END_CATCH_AND_BEGIN_CATCH_ALL

#define BEGIN_CATCH_ALL \
    catch (...)         \
    {

#define END_CATCH_ALL }

#endif
