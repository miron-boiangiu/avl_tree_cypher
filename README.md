<div id="top"></div>

[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/miron-boiangiu/avl_tree_cypher">
    <img src="images/logo.png" alt="Logo" width="120" height="120">
  </a>

<h3 align="center">Vigenere Cypher using AVL Tree</h3>

  <p align="center">
    Second Data Structures and Algorithms homework, requiring usage (and implementation) of an AVL tree to encrypt and decrypt messages. 
    <br />
    <br />
    <a href="https://github.com/miron-boiangiu/avl_tree_cypher/issues">Report Bug</a>
    ·
    <a href="https://github.com/miron-boiangiu/avl_tree_cypher/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Coded in C, this project contains an implementation of an AVL tree data structure and an example usage of it to encrypt messages, using the Vigenere Cypher.

Note: the AVL tree implemented also uses lists to deal with duplicate nodes, like in the picture below:
<br>
<a href="https://github.com/miron-boiangiu/avl_tree_cypher/blob/main/Tema2.pdf">Assignment link.</a>

TODO: add pic

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [C](http://www.open-std.org/jtc1/sc22/wg14/)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started



### Prerequisites

* make & gcc
  ```sh
  sudo apt-get install build-essential
  ```

### Compilation

1. Simply use the makefile included in order to compile the program.
   ```sh
   make
   ```
2. You can then run the checker by running ./run_checker.sh.
   ```sh
   ./run.sh
   ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage
Here is a quick rundown of the more important functions implemented:
<h2> AVL Tree implementation:</h2>
<br>

<details>
  <summary>avlRotate{Left,Right} (tree, node)</summary>

```
performs a simple tree rotation at the node specified.
```

</details>
<br>

<details>
  <summary>insert (tree, value, info)</summary>

```
a new node is added to the tree. If a node with the same value already exists, then this node is appended to the duplicate list for that node.
```

</details>
<br>

<details>
  <summary>delete(tree, value)</summary>

```
deletes the a node containing the value passes as a parameter. 
```

</details>
<br>

<h2>The Vigenere cypher:</h2>

A string that has to be encrypted has a Vigenere range (the offset list) created from it by turning each word into an AVL tree node and using one of
the following functions:

<br>

<details>
  <summary>inorderKeyQuery(tree)</summary>

```
creates a Vigenere range based on the order of apparition of words in the string.
```

</details>
<br>

<details>
  <summary>levelKeyQuery(tree)</summary>

```
creates a Vigenere range based on the nodes at the depth of the most frequent word.
```

</details>
<br>

<details>
  <summary>rangeKeyQuery(tree, a, b)</summary>

```
creates a Vigenere range that only contains the nodes whose values are between a and b.
```

</details>
<br>

Note: In case you find anything unclear or too vague, please consult the assignment link and README_OLD.

<br>
<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Boiangiu Victor-Miron - miron.boiangiu@gmail.com

Project Link: [https://github.com/miron-boiangiu/avl_tree_cypher](https://github.com/miron-boiangiu/avl_tree_cypher)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/miron-boiangiu/avl_tree_cypher.svg?style=for-the-badge
[contributors-url]:https://github.com/miron-boiangiu/avl_tree_cypher/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/miron-boiangiu/avl_tree_cypher.svg?style=for-the-badge
[forks-url]:https://github.com/miron-boiangiu/avl_tree_cypher/network/members
[stars-shield]: https://img.shields.io/github/stars/miron-boiangiu/avl_tree_cypher.svg?style=for-the-badge
[stars-url]:https://github.com/miron-boiangiu/avl_tree_cypher/stargazers
[issues-shield]: https://img.shields.io/github/issues/miron-boiangiu/avl_tree_cypher.svg?style=for-the-badge
[issues-url]:https://github.com/miron-boiangiu/avl_tree_cypher/issues
[license-shield]: https://img.shields.io/github/license/miron-boiangiu/avl_tree_cypher.svg?style=for-the-badge
[license-url]:https://github.com/miron-boiangiu/avl_tree_cypher/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
