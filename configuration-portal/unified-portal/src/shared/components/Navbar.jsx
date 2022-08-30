import { Link } from 'react-router-dom'
import { Navbar } from "flowbite-react";
// import FinwareLogo from '../../assets/finware-logo.png'

function NavbarLink({ to, ...props }) {
    return <Link to={to} className="block py-2 pr-4 pl-3 md:p-0 border-b border-gray-100  text-gray-700 hover:bg-gray-50 dark:border-gray-700 dark:text-gray-400 dark:hover:bg-gray-700 dark:hover:text-white md:border-0 md:hover:bg-transparent md:hover:text-blue-700 md:dark:hover:bg-transparent md:dark:hover:text-white">
        {props.children}
    </Link>;
}

export default function MyNavbar() {
    return <Navbar fluid={true} rounded={true}>
        <Link to="/">
            <div className="flex items-center">
                <img
                    src="https://flowbite.com/docs/images/logo.svg"
                    className="mr-3 h-6 sm:h-9"
                    alt="Finware Logo"
                />
                <span className="self-center whitespace-nowrap text-xl font-semibold dark:text-white font-quicksand">
                    Finware
                </span>
            </div>
        </Link>
        <Navbar.Toggle />
        <Navbar.Collapse>
            <NavbarLink to="/">Home</NavbarLink>
            <NavbarLink to="finance-accounts">Credit Cards</NavbarLink>
            <NavbarLink to="config">General Config</NavbarLink>
            <NavbarLink to="wifi">WiFi</NavbarLink>
        </Navbar.Collapse>
    </Navbar>

};
